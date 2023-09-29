#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include "types_inc.hpp"

class Graph {
    public:     Graph(int nVertices, int nExtraHotels, int numTrips, double tourLength, k_double tripLenghts, trip_matrix tour);
                ~Graph();
                int getNVertices();
                int getNExtraHotels();
                int getNumTrips();
                double getTourLength();
                k_double getTripLenghts();
                trip_matrix getTour();


    
    private:    int nVertices;
                int nExtraHotels;
                int numTrips;
                double tourLength;
                k_double tripLenghts;
                trip_matrix tour;
};

#endif  // GRAPH_H_