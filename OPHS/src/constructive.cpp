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

    tour_t Constructive::heuristic(Trip &t, unordered &availableLocations){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
       
        int nVertices = this->graph->getNVertices();

        int best_node = -1;

        //heuristic is score/distance
        double best_heuristic = 0;

        
        for(auto i : availableLocations){

            double heuristic = 0;

            heuristic = adjMatrix[t.locations.back()][i].score / adjMatrix[t.locations.back()][i].dist;

            if(heuristic > best_heuristic){
                best_heuristic = heuristic;
                best_node = i;
            }

        }

        t.locations.push_back(best_node);
        t.tripLength -= adjMatrix[t.locations.back()][best_node].dist;
        availableLocations.erase(best_node);
    }

    tour_t Constructive::greedySolution(){
        int trips = this->graph->getNumTrips();
 
        int aux_trip = 0;

        unordered availableLocations;
        availableLocations.reserve(this->graph->getNVertices());

        for(int i = 0; i < this->graph->getNVertices(); i++){
            availableLocations.insert(i);
        }

        for (int i = 0; i < iterations; i++)
        {
            this->heuristic(this->solution[aux_trip], availableLocations);
            if(this->solution[aux_trip].tripLength <= 0){
                aux_trip++;
            }
        }
        
    }

    
}