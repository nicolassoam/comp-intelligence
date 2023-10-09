#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
    Constructive::Constructive(Graph* graph, int iterations){
        this->graph = graph;
        this->iterations = iterations;
        this->solution.push_back(0);
    }

    tour_t Constructive::heuristic(unordered &visited){
        //OPHS PROBLEM HEURISTIC
        trip_matrix adjMatrix = this->graph->getAdjMatrix();
        double heuristic = 0;
        int n_vertices = this->graph->getNVertices();


        for (int i = 0; i < n_vertices; i++)
        {
            if(visited.find(i) == visited.end()){
                double min = 9999999;
                int min_index = -1;
                for (int j = 0; j < n_vertices; j++)
                {
                    if(visited.find(j) == visited.end()){
                        if(adjMatrix[i][j].dist < min){
                            min = adjMatrix[i][j].dist;
                            min_index = j;
                        }
                    }
                }
                visited.insert(min_index);
                this->solution.push_back(min_index);
                heuristic += min;
            }
        }
    }

    tour_t Constructive::greedySolution(){
    
        unordered visited;
        for (int i = 0; i < iterations; i++)
        {
            this->heuristic(visited);
        }
        
    }

    
}