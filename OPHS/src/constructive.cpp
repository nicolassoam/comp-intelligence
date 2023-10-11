#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
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
    }

    void Constructive::heuristic(Trip &t, set &availableLocations){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
       
        int nVertices = this->graph->getNVertices();

        int best_node = -1;

        //heuristic is score/distance
        double best_heuristic = 0;

        //trip must end in a hotel
        for(auto i : availableLocations){

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
        
        if(best_node == -1){
            return;
        }
        
        t.tripLength -= adjMatrix[t.locations.back()][best_node].dist;

        double previous_length = t.tripLength;
        std::cout << availableLocations.size() << std::endl;

        if(t.tripLength <= 0){
            t.tripLength = previous_length;
        
            std::cout << "entrou aqui" << std::endl;
            //find nearest hotel
            int nearest_hotel = -1;

            double best_dist = BIG;

            for(int i = 0; i < this->graph->getNExtraHotels() + 2; i++){
                if(adjMatrix[t.locations.back()][i].dist < best_dist){
                    best_dist = adjMatrix[t.locations.back()][i].dist;
                    nearest_hotel = i;
                }
            }

            t.locations.push_back(nearest_hotel);
            t.tripLength -= adjMatrix[t.locations.back()][nearest_hotel].dist;

            return;
        }

        t.locations.push_back(best_node);

        availableLocations.erase(best_node);
    }

    solution_t Constructive::greedySolution(){
        int trips = this->graph->getNumTrips();
 
        int aux_trip = 0;
        int available_trips = trips;
        set availableLocations;
       

        for(int i = 2 + this->graph->getNExtraHotels(); i < this->graph->getNVertices(); i++){
            availableLocations.insert(i);
        }

        for (int i = 0; i < iterations; i++)
        {
            if(availableLocations.size() == 0) break;

            this->heuristic(this->solution[aux_trip], availableLocations);
            
            // std::cout << this->solution[aux_trip].tripLength << std::endl;
            if((this->solution[aux_trip].tripLength <= 0) && (available_trips > 0)){
                aux_trip++;
                this->solution[aux_trip].locations.push_back(this->solution[aux_trip-1].locations.back());
                available_trips--;
            }
            
        }

        

        return this->solution;
        
    }

    
}