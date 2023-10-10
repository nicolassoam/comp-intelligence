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

        double best_score = 0;
        int best_node = -1;
        
        for(auto i : availableLocations){
            double score = 0;
            for(auto j : t.locations){
                score += adjMatrix[i][j].score;
            }

            if(score > best_score){
                best_score = score;
                best_node = i;
            }

            t.locations.push_back(best_node);
            availableLocations.erase(best_node);
        }
    }

    tour_t Constructive::greedySolution(){
        int trips = this->graph->getNumTrips();
        k_double tripLengths = this->graph->getTripLenghts();
        int aux_trip = 0;
        unordered availableLocations;
        availableLocations.reserve(this->graph->getNVertices());

        for(int i = 0; i < this->graph->getNVertices(); i++){
            availableLocations.insert(i);
        }

        for (int i = 0; i < iterations; i++)
        {
            this->heuristic(this->solution[aux_trip], availableLocations);
        }
        
    }

    
}