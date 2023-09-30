#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "types_inc.hpp"
#include "graph.hpp"

namespace Search{

    // a ideia é que cada set no vetor seja uma "trip"
    using solution_t = std::vector<std::unordered_set<int>>;
    
    class Constructive{
        public:     Constructive(Graph* graph, int iterations);
                    ~Constructive();
                    Graph* getGraph();
                    tour_t greedySolution();

        private:    Graph* graph;
                    solution_t solution;
                    int iterations;
                    int heuristic();
    };
}

#endif // CONSTRUCTIVE_H_