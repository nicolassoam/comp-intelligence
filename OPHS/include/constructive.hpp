#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "types_inc.hpp"
#include "graph.hpp"

namespace Search{

    // a ideia é que cada set no vetor seja uma "trip"
    using solution_t = std::vector<Trip>;
    using set  = std::set<int>;
    class Constructive{
        public:     Constructive(Graph* graph, int iterations);
                    ~Constructive();
                    Graph*          getGraph();
                    solution_t      greedySolution();

        private:    Graph*          graph;
                    solution_t      solution;
                    int             iterations;
                    set             availableLocations;
                    int             heuristic(Trip &t, trip_matrix &adjMatrix);
                    void            updateAvailableLocation(Trip &t, trip_matrix &adjMatrix, double &avTourLength);
                    int             findNearestHotel(trip_matrix &adjMatrix, int lastLocation);
                    void            lastTripConstructor(int iter, trip_matrix &adjMatrix, double avTourLength);

    };
}

#endif // CONSTRUCTIVE_H_