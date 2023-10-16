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
            std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << " " << std::get<3>(i) << std::endl;
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
        
        this->initialHotelSelection();
        std::cout << "Constructive created" << std::endl;
    }

    int nearestHotelToAllLocations(trip_matrix &adj, int nLocations, int nHotels){
        int nearest_hotel = -1;
        k_double hotel_dist(nHotels, 0);

        //starting from the last hotel inserted, calculate which hotel is closest to all locations
        for(int i = 0; i < nHotels; i++){
            double h_dist = 0;

            for(int j = nHotels; j < nLocations; j++){
                h_dist += adj[i][j].dist;
            }

            hotel_dist[i] = h_dist;

            if(hotel_dist[i] < hotel_dist[nearest_hotel]){
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

        Trip firstTrip;

        // inserts H0 as start of first trip
        firstTrip.locations.push_back(0);
        firstTrip.tripLength = firstTripLength;

        // inserts random hotel as end of first trip
        std::uniform_int_distribution<int> dist(0, hotelNum-1);
        int random_hotel = dist(gen_);
        firstTrip.locations.push_back(random_hotel);

        this->solution.push_back(firstTrip);

        // inserts random hotels to the other trips
        for(int i = 1; i < trips; i++){
            Trip intermediateTrip;

            intermediateTrip.locations.push_back(this->solution.back().locations.back());
            intermediateTrip.tripLength = tripLengths[i];

            // nearest_hotel = nearestHotelToAllLocations(adjMatrix, numLocations, hotelNum);
            random_hotel = dist(gen_);
            intermediateTrip.locations.push_back(random_hotel);
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
    
    bool verifyNeighbor (int iNode, int jNode, tour_t &locations) {
        for (auto i : locations) {
            if (i == iNode) {
                if (locations[i+1] == jNode)
                    return true;
            }
        }
        return false;
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

    void initCandidateList(list_t& candidateList,Trip& solution, trip_matrix& adjMatrix, int nLocations, int nHotels, set& firstLocations){
        int initialTripSize = solution.locations.size()-1;

        for(int i = 0; i < initialTripSize; i++){
            int iNode = solution.locations[i];
            int jNode = solution.locations[i+1];

            for(int k = nHotels; k < nLocations; k++){
                if(firstLocations.find(k) == firstLocations.end()){
                    double deltaDist = adjMatrix[iNode][k].dist + adjMatrix[k][jNode].dist - adjMatrix[iNode][jNode].dist;
                    double heuristic = deltaDist / adjMatrix[iNode][k].score;

                    candidateList.push_back(std::make_tuple(heuristic, k, iNode, jNode));
                }
            }
        }

        sortCandidateList(candidateList);
    }

    void Constructive::updateCandListForNewTrip(trip_matrix &adjMatrix, Trip* lastTrip, tour_t& removedLocations){
  
        double heuristic = 0;
        std::tuple<double, int, int, int> candidate;
        tour_t& locations = lastTrip->locations;
        std::vector<int> availableLocations;
        int candidateListSize = this->candidateList.size();
        
        for(auto cand : this->candidateList){
            availableLocations.push_back(std::get<1>(cand));
        }

        if(removedLocations.size() > 0){
            for(auto loc : removedLocations){
                availableLocations.push_back(loc);
            }
            removedLocations.clear();
        }
        
        this->candidateList.clear();
        

        for(int i = 0; i < locations.size()-1; i++){
            int iNode = locations[i];
            int jNode = locations[i+1];
            for(int i = 0; i < availableLocations.size(); i++){
                int kNode = availableLocations[i];

                double heuristic = adjMatrix[iNode][kNode].dist / adjMatrix[iNode][kNode].score;
                this->candidateList.push_back(std::make_tuple(heuristic, kNode, iNode, jNode));
            }
        }

        sortCandidateList(this->candidateList);
    }

    set Constructive::insertFirstLocations(solution_t &solution, trip_matrix &adjMatrix, int nHotels){
        std::set<int> firstLocations;

        int trips = this->graph->getNumTrips() - 1;
        int nLocations = this->graph->getNVertices() - nHotels;

        std::uniform_int_distribution<int> dist(nHotels-1, nLocations-1);

        for(int i = 0; i < trips; i++){
            bool inserted = false;

            while (!inserted) {
                int random_location = dist(gen_);

                // checks if length of trip is enough to add location
                double prevLength = adjMatrix[solution[i].locations.front()][solution[i].locations.back()].dist;
                double newLength = adjMatrix[solution[i].locations.front()][random_location].dist + adjMatrix[random_location][solution[i].locations.back()].dist;

                if (solution[i].tripLength + prevLength - newLength >= 0 && firstLocations.find(random_location) == firstLocations.end()) {

                    solution[i].tripLength += prevLength;
                    solution[i].tripLength -= newLength;
                    firstLocations.insert(random_location);
                    solution[i].locations.insert(solution[i].locations.begin() + 1, random_location);
                    inserted = true;

                }
            }
        }

        return firstLocations;
    }

    solution_t Constructive::greedySolution(){
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
        int trips = this->graph->getNumTrips() - 1;
 
        int aux_trip = 0;
        int available_trips = trips;
        double availableTourLength = this->graph->getTourLength();

        int nHotels = this->graph->getNExtraHotels() + 2;

        // insert random first location for all trips
        set firstLocations = insertFirstLocations(this->solution, adjMatrix, nHotels);

        initCandidateList(this->candidateList, this->solution[0], adjMatrix, this->graph->getNVertices(), nHotels, firstLocations);        

        tour_t removedLocations;

        for(int i = 0; i < this->iterations; i++){

            Trip* lastTrip = &this->solution[aux_trip];
            
            // end of current trip
            if(lastTrip->tripLength >= 0 && lastTrip->tripLength <= 3.5){
                aux_trip++;
                  
                if(available_trips > 0)
                    available_trips--;
                
                // end of tour
                if(aux_trip > trips){
                    //std::cout << "No more trips" << std::endl;
                    break;
                }

                updateCandListForNewTrip(adjMatrix, &this->solution[aux_trip], removedLocations);
                // printCandidateList(this->candidateList);
            }

            if(candidateList.empty()) {
                //std::cout << "No more candidates" << std::endl;
                break;
            } 

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

                //std::cout << "não dá pra remover (" << iNode << "," << jNode << ") para inserir " << kNode << std::endl;
                this->candidateList.erase(this->candidateList.begin());
                removedLocations.push_back(kNode);
                
                // retryHeuristic(this->candidateList.front(), adjMatrix, lastTrip->locations);
        
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