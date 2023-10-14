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
using rand_gen = std::mt19937;
class SA {
    public:     SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels);
                ~SA();
                void run(solution_t& initialSolution, tour_t& unvisitedLocations);
                void printSolution();
                
    private:    Neighbor bestSolution;
                Neighbor currentSolution;
                rand_gen rng;
                trip_matrix adjMatrix;
                double initialTemperature;
                double finalTemperature;
                double coolingFactor;
                int iterationsPerTemperature;
                int nHotels;
                double objectiveFunction(solution_t& solution);
                Neighbor generateNeighbor(solution_t currentSolution, tour_t& unvisitedLocations);
                void swapInTrip(solution_t& solution);
                void swapHotels(solution_t& solution);
                void swapBetweenTrips(solution_t& solution);
                void swapBetweenTrips2(solution_t& solution);
                solution_t insertIfFeasible(solution_t solution, tour_t& unvisitedLocations);
};



#endif // SA_H_  
