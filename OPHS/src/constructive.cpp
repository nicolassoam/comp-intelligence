#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {

    void printAvailableLocations(set &availableLocations){
        std::cout << "AL: ";
        for(auto i : availableLocations){
            std::cout << i << " ";
        }
        std::cout << std::endl;
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

        std::cout << "Constructive created" << std::endl;
    }

    int Constructive::heuristic(Trip &t, trip_matrix &adjMatrix){
        
        //heuristic is score/distance
        double best_heuristic = 0;

        int best_node = -1;

        //trip must end in a hotel
        for(auto i : this->availableLocations){

            double heuristic = 0;
            
            if(adjMatrix[t.locations.back()][i].score == 0){
                continue;
            }
            
            heuristic = adjMatrix[t.locations.back()][i].score / adjMatrix[t.locations.back()][i].dist;
           
            if(heuristic > best_heuristic){
                best_heuristic = heuristic;
                best_node = i;
            }
        }

        std::cout << "Best node: " << best_node << std::endl;

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

        std::cout << "Nearest hotel: " << nearest_hotel << std::endl;
        return nearest_hotel;
    }

    void Constructive::updateAvailableLocation(Trip &t, trip_matrix &adjMatrix, double &avTourLength){
       
        int nVertices = this->graph->getNVertices();
        int best_node = this->heuristic(t, adjMatrix);
        double previous_length = t.tripLength;
        double additionalTripLength = adjMatrix[t.locations.back()][best_node].dist;
        t.tripLength -= additionalTripLength;

        if(t.tripLength <= 0){
            t.tripLength = previous_length;
        
            int nearest_hotel = this->findNearestHotel(adjMatrix, t.locations.back());

            additionalTripLength = adjMatrix[t.locations.back()][nearest_hotel].dist;

            t.locations.push_back(nearest_hotel);
            t.tripLength -= additionalTripLength;
            t.finished = true;
            
            avTourLength -= additionalTripLength;
            return;
        }

        t.locations.push_back(best_node);
        avTourLength -= additionalTripLength;
        std::cout << "Node " << best_node << " added to trip." << std::endl;
        this->availableLocations.erase(best_node);
    }

    solution_t Constructive::greedySolution(){
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
        int trips = this->graph->getNumTrips() - 1;
 
        int aux_trip = 0;
        int available_trips = trips;
        double availableTourLength = this->graph->getTourLength();

        for(int i = 2 + this->graph->getNExtraHotels(); i < this->graph->getNVertices(); i++){
            this->availableLocations.insert(i);
        }

        int iter;

        for (iter = 0; iter < this->iterations; iter++)
        {   
            // if it is the last trip, interrupts iteration
            if (available_trips == 0)
                break;

            this->updateAvailableLocation(this->solution[aux_trip], adjMatrix, availableTourLength);
            printAvailableLocations(this->availableLocations);
            printTrip(&this->solution[aux_trip]);
            
            
            if((this->solution[aux_trip].finished) && (available_trips > 0)){
                aux_trip++;
                std::cout << "----------------------------" << std::endl << "Available trips: " << available_trips << std::endl;
                std::cout << "New trip created" << std::endl;
                this->solution[aux_trip].locations.push_back(this->solution[aux_trip-1].locations.back());
                available_trips--;
            }
        }

        this->lastTripConstructor(iter, adjMatrix, availableTourLength);

        return this->solution;
    }

    void sortCandidateList(list_t &candidateList){
        std::sort(candidateList.begin(), candidateList.end(), [](auto &left, auto &right) {
            return std::get<0>(left) > std::get<0>(right);
        });
    }

    void Constructive::setToCandidateList(list_t &candidateList, trip_matrix& adjMatrix, int firstHotel){
        double heuristic = 0;

        // first candidate will be location closest to hotel 1 (last hotel)
        for(auto i : this->availableLocations){
            heuristic = adjMatrix[1][i].score / adjMatrix[1][i].dist;
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

            candidateList[n] = aux;
        }

        sortCandidateList(candidateList);
    }

    void retryHeuristic(tuple &candidate, trip_matrix &adjMatrix, tour_t &locations) {
        double heuristic = BIG;
        int iNode = std::get<2>(candidate), jNode = std::get<3>(candidate);
        for (int i = 0; i < locations.size(); i++) {
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
        
        // add hotel 1 (last hotel) to the last trip
        lastTrip->locations.push_back(1);  

        this->setToCandidateList(candidateList, adjMatrix, firstHotel);
        printCandidateList(candidateList);

        // goes through candidates list and adds best location to best position in last trip
        for (iter = iter; iter < this->iterations; iter++) {

            // node k is added between node i and node j
            int kNode = std::get<1>(candidateList.front());
            int iNode = std::get<2>(candidateList.front());
            int jNode = std::get<3>(candidateList.front());

            // verify if edge (i,j) can be removed to add edges (i,k) and (k,j)
            double prevLength = adjMatrix[iNode][jNode].dist;
            double newLength = adjMatrix[iNode][kNode].dist + adjMatrix[kNode][jNode].dist;

            if (lastTrip->tripLength + prevLength - newLength >= 0) {

                lastTrip->tripLength += prevLength;
                lastTrip->tripLength -= newLength;

                // find iNode index in lastTrip
                std::vector<int>::iterator it = std::find(lastTrip->locations.begin(), lastTrip->locations.end(), iNode);

                lastTrip->locations.insert(it + 1, kNode);
                std::cout << "Node " << kNode << " added between " << iNode << " and " << jNode << std::endl;

                // updates candidate list
                this->updateCandidateList(candidateList, adjMatrix, kNode, lastTrip);
                printCandidateList(candidateList);
                std::cout << std::endl;
                printTrip(lastTrip);

            } else {
                std::cout << "não dá pra remover (" << iNode << "," << jNode << ") para inserir " << kNode << std::endl;
                candidateList.erase(candidateList.begin());
                retryHeuristic(candidateList.front(), adjMatrix, lastTrip->locations);
                sortCandidateList(candidateList);
            }

            if(lastTrip->tripLength <= 0 || candidateList.empty()) {
                std::cout << "Last trip finished" << std::endl;
                break;
            }
        }
    }
}