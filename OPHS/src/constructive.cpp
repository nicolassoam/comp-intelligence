#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
    Constructive::Constructive(Graph* graph, int iterations){
        this->graph = graph;
        this->iterations = iterations;

        tour_t aux;

        aux.push_back(0);

        this->solution.push_back(aux);

        for(int i = 1; i < graph->getNumTrips(); i++){
            tour_t aux;
            this->solution.push_back(aux);
        }
    }

    tour_t Constructive::heuristic(tour_t &tour, unordered &availableLocations){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
       
        int nVertices = this->graph->getNVertices();

        double best_score = 0;
        int best_node = -1;
        
        for(auto i : availableLocations){
            double score = 0;
            for(auto j : tour){
                score += adjMatrix[i][j].score;
            }

            if(score > best_score){
                best_score = score;
                best_node = i;
            }

            tour.push_back(best_node);
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