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
        std::cout << "Node " << best_node << " added to trip. Length: " << t.tripLength << std::endl;
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
            
            // std::cout << this->solution[aux_trip].tripLength << std::endl;
            if((this->solution[aux_trip].finished) && (available_trips > 0)){
                aux_trip++;
                std::cout << "Available trips: " << available_trips << std::endl;
                std::cout << "New trip created" << std::endl;
                this->solution[aux_trip].locations.push_back(this->solution[aux_trip-1].locations.back());
                available_trips--;
            }
        }

        this->lastTripConstructor(iter, adjMatrix, availableTourLength);

        return this->solution;
    }

    void Constructive::setToCandidateList(queue_t &candidateList, trip_matrix& adjMatrix){
        double heuristic = 0;

        for(auto i : this->availableLocations){
            heuristic = adjMatrix[1][i].score / adjMatrix[1][i].dist;
            candidateList.push(std::make_tuple(heuristic, i, 1));
        }
    }

    void Constructive::lastTripConstructor(int iter, trip_matrix &adjMatrix, double avTourLength) {
        Trip* lastTrip = &this->solution.back();
        lastTrip->locations.push_back(1);           // add hotel 1 (last hotel) to the last trip
        queue_t candidateList;

        this->setToCandidateList(candidateList, adjMatrix);

        for (iter = iter; iter < this->iterations; iter++) {
            
        }
    }
}