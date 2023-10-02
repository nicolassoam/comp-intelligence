#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
    Constructive::Constructive(Graph* graph, int iterations){
        this->graph = graph;
        this->iterations = iterations;
        this->solution.resize(graph->getNumTrips());

        tour_t aux;

        aux.push_back(0);

        this->solution.push_back(aux);

        for(int i = 1; i < graph->getNumTrips(); i++){
            tour_t aux;
            this->solution.push_back(aux);
        }
    }

    tour_t Constructive::heuristic(tour_t &tour, unordered &visited, double trip_length){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
        double tmax = this->graph->getTourLength();
        int nVertices = this->graph->getNVertices();

        while(trip_length > 0){
            int min = INT_MAX;
            int min_index = -1;

            for(int i = 0; i < nVertices; i++){
                if(visited.find(i) == visited.end()){
                    if(adjMatrix[tour.back()][i].dist < min){
                        min = adjMatrix[tour.back()][i].dist;
                        min_index = i;
                    }
                }
            }

            if(min_index == -1){
                break;
            }

            tour.push_back(min_index);
            visited.insert(min_index);
            trip_length -= min;
        }

    }

    tour_t Constructive::greedySolution(){
        int trips = this->graph->getNumTrips();
        k_double tripLengths = this->graph->getTripLenghts();
        int aux_trip = 0;

        for (int i = 0; i < iterations; i++)
        {
            unordered visited;
            this->heuristic(this->solution[aux_trip], visited, tripLengths[aux_trip]);
        }
        
    }

    
}