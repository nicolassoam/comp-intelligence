#include "../include/constructive.hpp"

// OPHS Constructive greedy
namespace Search {
    Constructive::Constructive(Graph* graph, int iterations){
        this->graph = graph;
        this->iterations = iterations;
        std::unordered_set<int> aux;
        aux.insert(0);
        this->solution.push_back(aux);
    }

    int Constructive::heuristic(){
        //OPHS PROBLEM HEURISTIC

    }

    tour_t Constructive::greedySolution(){
        
    }

    
}