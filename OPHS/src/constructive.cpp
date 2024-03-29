#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
    static const int seed = time(0);

    std::mt19937 gen_(seed);

    void calculateTourScore(trip_matrix& adjMatrix, solution_t &solution) {
        double score = 0;
        int t = 1;
        std::cout << "Solution: " << std::endl;
        for (auto i : solution) {
            std::cout << "Trip " << t;
            std::cout << " | Length: " << i.tripLength << " | ";

            for (int j = 0; j < i.locations.size()-1; j++) {
                score += adjMatrix[i.locations[j]][i.locations[j+1]].score;
                std::cout << i.locations[j] << " ";
            }
            t++;
            std::cout << i.locations.back() << std::endl;
        }
        std:: cout << "Tour Score: " << score << std::endl;
    }

    void printCandidateList(list_t &candidateList) {
        std::cout << "Candidate List: " << std::endl;
        for (auto i : candidateList) {
            std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << "/";
        }
        std::cout << std::endl;
    }

    void printTrip(Trip* trip) {
        std::cout << "Trip: ";
        for (auto i : trip->locations) {
            std::cout << i << " ";
        }
        std::cout << " | Av. Length: "<< trip->tripLength << std::endl;
    }

    Constructive::Constructive(Graph* graph, int iterations){
        this->graph = graph;
        this->iterations = iterations;
        k_double tripLengths = this->graph->getTripLenghts();
        double trip_0 = tripLengths[0];
        Trip aux;

        aux.locations.push_back(0);
        aux.tripLength = trip_0;

        this->solution.push_back(aux);

        for(int i = 1; i < graph->getNumTrips(); i++){
            Trip aux;
            aux.tripLength = tripLengths[i];
            this->solution.push_back(aux);
        }

        unsigned int seed = time(0);
        std::cout << "Seed: " << seed << std::endl;
        srand(seed);

        this->rng = rand_gen(seed);

        std::cout << "Constructive created" << std::endl;
    }

    int Constructive::heuristic(Trip &t, trip_matrix &adjMatrix){
        double best_heuristic = 0;
        int best_node = -1;
        
        //trip must end in a hotel
        for(auto i : this->availableLocations){

            double heuristic = 0;
            
            if(adjMatrix[t.locations.back()][i].score == 0)
                continue;
            
            heuristic = adjMatrix[t.locations.back()][i].score / adjMatrix[t.locations.back()][i].dist;
            
            if(heuristic > best_heuristic){
                best_heuristic = heuristic;
                best_node = i;
            }
        }

        return best_node;
    }

    int Constructive::randomHeuristic(Trip &t, trip_matrix &adjMatrix){
        
        // inicializa vetor com três itens
        std::vector<std::pair<double, int>> nodes;
        nodes.push_back(std::make_pair(0, -1));
        
        //trip must end in a hotel
        for(auto i : this->availableLocations){

            double heuristic = 0;
            
            if(adjMatrix[t.locations.back()][i].score == 0)
                continue;
            
            heuristic = adjMatrix[t.locations.back()][i].score / adjMatrix[t.locations.back()][i].dist;
            
            if(heuristic > nodes.back().first){
                nodes.push_back(std::make_pair(heuristic, i));
                std::sort(nodes.begin(), nodes.end(), std::greater<std::pair<double, int>>());
            }
        }

        // retorna aleatoriamente um dos três melhores nós
        std::uniform_int_distribution<int> dist(0, 2);
        int random = dist(this->rng);
        int best_node;

        if (nodes[random].first > 0)
            best_node = nodes[random].second;
        else
            best_node = nodes[0].second;

        return best_node;
    }

    int Constructive::findNearestHotel(trip_matrix &adjMatrix, int lastLocation){
        double best_dist = BIG;
        int nearest_hotel = -1;

        for(int i = 0; i < this->graph->getNExtraHotels() + 2; i++){
            if(adjMatrix[lastLocation][i].dist < best_dist){
                best_dist = adjMatrix[lastLocation][i].dist;
                nearest_hotel = i;
            }
        }

        return nearest_hotel;
    }

    void Constructive::initialHotelSelection(){
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
        k_double tripLengths = this->graph->getTripLenghts();

        double firstTripLength = tripLengths[0];

        int hotelNum = 2 + this->graph->getNExtraHotels();

        int numLocations = this->graph->getNVertices() - hotelNum;

        int trips = this->graph->getNumTrips() - 1;

        int aux_trip = 0;
        int available_trips = trips;
        double availableTourLength = this->graph->getTourLength();

        for(int i = hotelNum; i < numLocations + hotelNum; i++){
            availableLocations.push_back(i);
        }

        // inserts H0 as start of first trip
        firstTrip.locations.push_back(0);
        firstTrip.tripLength = firstTripLength;

        // inserts random hotel as end of first trip
        std::uniform_int_distribution<int> dist(0, hotelNum-1);
        int random_hotel = dist(gen_);
        firstTrip.locations.push_back(random_hotel);

        firstTrip.tripLength -= adjMatrix[0][random_hotel].dist;
        
        this->solution.push_back(firstTrip);

        // inserts random hotels to the other trips
        for(int i = 1; i < trips; i++){
            Trip intermediateTrip;

            intermediateTrip.locations.push_back(this->solution.back().locations.back());
            intermediateTrip.tripLength = tripLengths[i];

            // nearest_hotel = nearestHotelToAllLocations(adjMatrix, numLocations, hotelNum);
            random_hotel = dist(gen_);
            intermediateTrip.locations.push_back(random_hotel);
            intermediateTrip.tripLength -= adjMatrix[intermediateTrip.locations.front()][random_hotel].dist;
            this->solution.push_back(intermediateTrip);
        }

        // inserts end hotel of second to last trip as start of last trip
        Trip lastTrip;
        lastTrip.locations.push_back(this->solution.back().locations.back());
        lastTrip.tripLength = tripLengths[trips-1];

        // inserts H1 as end of last trip
        lastTrip.locations.push_back(1);

        this->solution.push_back(lastTrip);
    }

    void sortCandidateList(list_t &candidateList){
        std::sort(std::execution::par_unseq,candidateList.begin(), candidateList.end(), [](auto &left, auto &right) {
            return std::get<0>(left) < std::get<0>(right);
        });
    }

    void Constructive::setToCandidateList(list_t &candidateList, trip_matrix& adjMatrix, int firstHotel){
        double heuristic = 0;

        // first candidate will be location closest to hotel 1 (last hotel)
        for(auto i : this->availableLocations){
            heuristic = adjMatrix[firstHotel][i].dist / adjMatrix[firstHotel][i].score + adjMatrix[i][1].dist / adjMatrix[i][1].score;
            candidateList.push_back(std::make_tuple(heuristic, i, firstHotel, 1));
        }

        sortCandidateList(candidateList);
    }
    
    bool verifyNeighbor (int iNode, int jNode, tour_t &locations) {
        for (auto i : locations) {
            if (i == iNode) {
                if (locations[i+1] == jNode)
                    return true;
            }
        }
        return false;
    }

    void Constructive::updateCandidateList(list_t &candidateList, trip_matrix &adjMatrix, int kNode, Trip* lastTrip){
        double heuristic;
        std::tuple<double, int, int, int> aux;
        tour_t locations = lastTrip->locations;

        // remove kNode from candidate list
        candidateList.erase(candidateList.begin());

        for (int n = 0; n < candidateList.size(); n++) {
            aux = candidateList[n];
            heuristic = std::get<0>(aux);
            int c = std::get<1>(aux);
            int i = std::get<2>(aux);
            int j = std::get<3>(aux);

            // if current best insertion is on the edge last location was inserted, it is not possible anymore
            if (!verifyNeighbor(i, j, locations))
                heuristic = -1;

            // calculates heuristic for candidate c being inserted between (i,kNode)
            double deltaDist = adjMatrix[i][c].dist + adjMatrix[c][kNode].dist - adjMatrix[i][kNode].dist;
            double deltaScore = adjMatrix[i][c].score  - adjMatrix[i][kNode].score;
            double newHeuristic = deltaDist / deltaScore;

            if (newHeuristic > heuristic)
                heuristic = newHeuristic;

            // calculates heuristic for candidate c being inserted between (kNode,j)
            deltaDist = adjMatrix[kNode][c].dist + adjMatrix[c][j].dist - adjMatrix[kNode][j].dist;
            deltaScore = adjMatrix[kNode][c].score - adjMatrix[kNode][j].score;
            newHeuristic = deltaDist / deltaScore;

            if (newHeuristic > heuristic)
                heuristic = newHeuristic;

            std::get<0>(aux) = heuristic;
            std::get<2>(aux) = i;
            std::get<3>(aux) = kNode;

            candidateList[n] = aux;
        }

        sortCandidateList(candidateList);
    }

    void retryHeuristic(tuple &candidate, trip_matrix &adjMatrix, tour_t &locations) {
        double heuristic = BIG;
        int iNode = std::get<2>(candidate), jNode = std::get<3>(candidate);
        for (int i = 0; i < locations.size()-1; i++) {
            double newHeuristic = adjMatrix[locations[i]][std::get<1>(candidate)].dist / adjMatrix[locations[i]][std::get<1>(candidate)].score;
            if (newHeuristic < heuristic) {
                heuristic = newHeuristic;
                iNode = locations[i];
                jNode = locations[i+1];
            }
        }
        std::get<0>(candidate) = heuristic;
        std::get<2>(candidate) = iNode;
        std::get<3>(candidate) = jNode;
    }

    void Constructive::lastTripConstructor(int iter, trip_matrix &adjMatrix, double avTourLength) {
        Trip* lastTrip = &this->solution.back();
        list_t candidateList;
        
        int tripIndex = this->solution.size() - 1;
        int firstHotel = lastTrip->locations.back();                 
        int randomLocation = 0;
        // add hotel 1 (last hotel) to the last trip
        lastTrip->locations.push_back(1);  

        // add random location to the last trip
        bool missingFirstLocation = true;
        while (missingFirstLocation) {
            std::uniform_int_distribution<int> dist(0, this->availableLocations.size()-1);
            int random = dist(this->rng);
            auto r = this->availableLocations.begin();
            std::advance(r, random);
            randomLocation = std::distance(this->availableLocations.begin(), r);
            double prevLength = adjMatrix[lastTrip->locations.front()][firstHotel].dist;
            double newLength = adjMatrix[lastTrip->locations.front()][randomLocation].dist + adjMatrix[randomLocation][firstHotel].dist;

            if (lastTrip->tripLength + prevLength - newLength >= 0) {
                lastTrip->tripLength += prevLength;
                lastTrip->tripLength -= newLength;

                lastTrip->locations.insert(lastTrip->locations.begin()+1, randomLocation);
                this->availableLocations.erase(randomLocation);
                // updateCandidateList(candidateList, adjMatrix, randomLocation, lastTrip);
                missingFirstLocation = false;
            } 
        }

        this->setToCandidateList(candidateList, adjMatrix, randomLocation);

        // goes through candidates list and adds best location to best position in last trip
        for (iter = iter; iter < this->iterations; iter++) {

            // node k is added between node i and node j
            std::tuple<double,int,int,int> candidate = this->candidateList.front();
            int kNode = std::get<1>(candidate);
            int iNode = std::get<2>(candidate);
            int jNode = std::get<3>(candidate);

            // verify if edge (i,j) can be removed to add edges (i,k) and (k,j)
            double prevLength = adjMatrix[iNode][jNode].dist;
            double newLength = adjMatrix[iNode][kNode].dist + adjMatrix[kNode][jNode].dist;

            if (lastTrip->tripLength + prevLength >= newLength) {
                std::cout << "Trip num: " << aux_trip << std::endl; 
                lastTrip->tripLength += prevLength;
                lastTrip->tripLength -= newLength;

                // find iNode index in lastTrip
                std::vector<int>::iterator it = std::find(lastTrip->locations.begin(), lastTrip->locations.end(), iNode);

                lastTrip->locations.insert(it+1, kNode);
                //std::cout << "Node " << kNode << " added between " << iNode << " and " << jNode << std::endl;

                // updates candidate list
                this->updateCandidateList(this->candidateList, adjMatrix, kNode, lastTrip);

                // printCandidateList(this->candidateList);
                std::cout << std::endl;
                printTrip(lastTrip);

            } else {
                // remove candidatw from list
                this->candidateList.erase(this->candidateList.begin());
                removedLocations.push_back(kNode);
                
                sortCandidateList(candidateList);
            }

        }

        calculateTourScore(adjMatrix, this->solution);

        return this->solution;
    }

    void Constructive::updateCandidateList(list_t &candidateList, trip_matrix &adjMatrix, int kNode, Trip* lastTrip){
        double heuristic;
        std::tuple<double, int, int, int> aux;
        tour_t locations = lastTrip->locations;

        set candidateIndexes;

        // remove kNode from candidate list

        for (int i = 0; i < candidateList.size(); i++) {
            if (std::get<1>(candidateList[i]) == kNode) {
                candidateIndexes.insert(i);
            }
        }

        for (auto i : candidateIndexes) {
            candidateList.erase(candidateList.begin() + i);
        }

        for (int n = 0; n < this->candidateList.size(); n++) {
            aux = this->candidateList[n];
            heuristic = std::get<0>(aux);
            int c = std::get<1>(aux);
            int i = std::get<2>(aux);
            int j = std::get<3>(aux);

            // if current best insertion is on the edge last location was inserted, it is not possible anymore
            if (!verifyNeighbor(i, j, locations))
                heuristic = BIG;

            // calculates heuristic for candidate c being inserted between (i,kNode)
            double deltaDist = adjMatrix[i][c].dist + adjMatrix[c][kNode].dist - adjMatrix[i][kNode].dist;
            double deltaScore = adjMatrix[i][c].score + adjMatrix[c][kNode].score - adjMatrix[i][kNode].score;
            double newHeuristic = deltaDist / deltaScore;

            if (newHeuristic < heuristic)
                heuristic = newHeuristic;

            // calculates heuristic for candidate c being inserted between (kNode,j)
            deltaDist = adjMatrix[kNode][c].dist + adjMatrix[c][j].dist - adjMatrix[kNode][j].dist;
            deltaScore = adjMatrix[kNode][c].score + adjMatrix[c][j].score - adjMatrix[kNode][j].score;
            newHeuristic = deltaDist / deltaScore;

            if (newHeuristic < heuristic)
                heuristic = newHeuristic;

            std::get<0>(aux) = heuristic;
            std::get<2>(aux) = i;
            std::get<3>(aux) = kNode;

            this->candidateList[n] = aux;
        }

        sortCandidateList(candidateList);
    }

    tour_t Constructive::getFullTour() {
        tour_t fullTour;
        for (auto i : this->solution) {
            for (auto j : i.locations) {
                fullTour.push_back(j);
            }
        }
        return fullTour;
    }

    std::set<int> Constructive::getUnusedLocations() {
        std::set<int> unused;
        tour_t fullTour = this->getFullTour();
        for(int i = 2 + graph->getNExtraHotels(); i < graph->getNVertices() + graph->getNExtraHotels(); i++){
            if(std::find(fullTour.begin(), fullTour.end(), i) == fullTour.end()) {
                unused.insert(i);
            }
        }
        return unused;
    }
}