#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_
#include <iostream>
#include <vector>
#include <string>
#include "types_inc.hpp"
#include "graph.hpp"

namespace Constructive{
    class Constructive{
        public:     Constructive(Graph* graph);
                    ~Constructive();
                    Graph* getGraph();
                    tour_t greedySolution();

        private:    Graph* graph;
                    tour_t solution;
                    int heuristic();
    };
}

#endif // CONSTRUCTIVE_H_