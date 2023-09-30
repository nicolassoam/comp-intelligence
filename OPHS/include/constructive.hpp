#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_
#include <iostream>
#include <vector>
#include <string>
#include "types_inc.hpp"
#include "graph.hpp"

namespace Search{
    class Constructive{
        public:     Constructive(Graph* graph, int iterations);
                    ~Constructive();
                    Graph* getGraph();
                    tour_t greedySolution();

        private:    Graph* graph;
                    tour_t solution;
                    int iterations;
                    int heuristic();
    };
}

#endif // CONSTRUCTIVE_H_