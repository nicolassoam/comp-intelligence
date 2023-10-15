#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "types_inc.hpp"
#include "graph.hpp"
#include <queue>
#include <algorithm>
#include <execution>
#include <tuple>
#include <random>
namespace Search{

    // a ideia é que cada set no vetor seja uma "trip"
    using solution_t = std::vector<Trip>;
    using set  = std::set<int>;
    using list_t = std::vector<std::tuple<double,int,int, int>>;     // <heuristic, kNode, iNode, jNode> -> kNode being inserted in (iNode, jNode)
    using tuple = std::tuple<double,int,int, int>;
    using rand_gen = std::mt19937;
    class Constructive{
        public:     Constructive(Graph* graph, int iterations);
                    ~Constructive();
                    Graph*          getGraph();
                    solution_t      greedySolution();
                    std::set<int>   getUnusedLocations();

        private:    Graph*          graph;
                    solution_t      solution;
                    int             iterations;
                    set             availableLocations;
                    rand_gen        rng;
                    int             heuristic(Trip &t, trip_matrix &adjMatrix);
                    int             randomHeuristic(Trip &t, trip_matrix &adjMatrix);
                    void            updateAvailableLocation(Trip &t, trip_matrix &adjMatrix, double &avTourLength);
                    int             findNearestHotel(trip_matrix &adjMatrix, int lastLocation);
                    void            lastTripConstructor(int iter, trip_matrix &adjMatrix, double avTourLength);
                    void            setToCandidateList(list_t &candidateList, trip_matrix& adjMatrix, int firstHotel);
                    void            updateCandidateList(list_t &candidateList, trip_matrix &adjMatrix, int kNode, Trip* lastTrip);
                    tour_t          getFullTour();
    };
}

#endif // CONSTRUCTIVE_H_