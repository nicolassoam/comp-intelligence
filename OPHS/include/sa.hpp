#ifndef SA_H_
#define SA_H_
// SIMULATED ANNEALING
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include "types_inc.hpp"

using nH = std::vector<Neighbor>;
using solution_t = std::vector<Trip>;
class SA {
    public:     SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels);
                ~SA();
                void run(solution_t& initialSolution);
                void printSolution();
                
    private:    nH neighborhood;
                Neighbor bestSolution;
                Neighbor currentSolution;
                trip_matrix adjMatrix;
                double initialTemperature;
                double finalTemperature;
                double coolingFactor;
                int iterationsPerTemperature;
                int nHotels;
                double objectiveFunction(solution_t& solution);
                Neighbor generateNeighbor(solution_t currentSolution);
                void swapInTrip(solution_t& solution);
                void swapBetweenTrips(solution_t& solution);
};



#endif // SA_H_  
