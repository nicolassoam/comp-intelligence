#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include "types_inc.hpp"

class Graph {
    public:     Graph(int nVertices, int nExtraHotels, int numTrips, double tourLength, k_double tripLenghts);
                ~Graph();
                int getNVertices();
                int getNExtraHotels();
                int getNumTrips();
                double getTourLength();
                k_double getTripLenghts();
                trip_matrix getAdjMatrix();
                static double euclideanDistance(double x1, double y1, double x2, double y2);
                void toGraphMatrix(matrix_t& tour);
                void printGraph();

    private:    int nVertices;
                int nExtraHotels;
                int numTrips;
                double tourLength;
                k_double tripLenghts;
                trip_matrix adjMatrix;
                
};

#endif  // GRAPH_H_