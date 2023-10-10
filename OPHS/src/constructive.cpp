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

    tour_t Constructive::heuristic(tour_t &tour, unordered &visited){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
       
        int nVertices = this->graph->getNVertices();

        double best_score = 0;
        int best_node = -1;
        
        if(tour.size() == 0){
            for(int i = 0; i < nVertices; i++){
                if(visited.find(i) == visited.end()){
                    if(adjMatrix[0][i].score/adjMatrix[0][i].dist > best_score){
                        best_score = adjMatrix[0][i].score/adjMatrix[0][i].dist;
                        best_node = i;
                    }
                }
            }
        }else{
            for(int i = 0; i < nVertices; i++){
                if(visited.find(i) == visited.end()){
                    if(adjMatrix[tour.back()][i].score/adjMatrix[tour.back()][i].dist > best_score){
                        best_score = adjMatrix[tour.back()][i].score/adjMatrix[tour.back()][i].dist;
                        best_node = i;
                    }
                }
            }
        }
    }

    tour_t Constructive::greedySolution(){
        int trips = this->graph->getNumTrips();
        k_double tripLengths = this->graph->getTripLenghts();
        int aux_trip = 0;

        for (int i = 0; i < iterations; i++)
        {
            unordered visited;
            this->heuristic(this->solution[aux_trip], visited);
        }
        
    }

    
}