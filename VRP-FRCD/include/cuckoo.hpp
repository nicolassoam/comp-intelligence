#ifndef CUCKOO_HPP_
#define CUCKOO_HPP_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <numbers>  //defines phi, pi
#include <queue>
#include <tuple>
#include "instance.hpp"
#include "vehicle.hpp"

/*
    Modified cuckoo search algorithm for the VRP-FRCD problem
*/
//TODO: adapt the cuckoo struct to the VRP-FRCD problem
//TODO: adapt the MCS class to the VRP-FRCD problem
//TODO: adapt the solution_t type to the VRP-FRCD problem

using solution_t = std::vector<double>;
using v = std::vector<Vehicle>;

struct cuckoo{
    double      fitness;
    solution_t  solution;
    v           vehicles;
    int         usedVehicles = 0;
};

using nest_t = std::vector<cuckoo>;                  // population of cuckoos (one per nest)
using candidate = std::tuple<double,int,int,int>;    // heuristic, k, i, j
using list_t = std::vector<candidate>;               // list of candidates (heuristic, k, i, j)

class MCS{
    public:     MCS(int nNests, int nVehicles, int nIterations,solution_t lowerBound, solution_t upperBound, Instance* inst);
                double  fitness(solution_t solution);  
                void    search();
                void    printSolution();
                double  fitness2(v vehicles, int usedVehicles);
                ~MCS();

    private:    double phi = std::numbers::phi;
                int nNests;
                solution_t lowerBound;
                solution_t upperBound;
                Instance* inst;
                int nVehicles;
                int nIterations;
                double maxLevyStepSize = 1;
                double levyStepSize;
                double pa = 0.25;
                nest_t nests;
                // choose a random cuckoo via levy flight
                double levyFlight();
                cuckoo applyMoviment(cuckoo c, std::vector<double>iteratorVector);
                void initPopulation();
                void initPopulation2();
                void supplierInit(cuckoo& cuckoo);
                void retailerInit(cuckoo& cuckoo);
                void outletInit(cuckoo& cuckoo);
};




#endif // CUCKOO_HPP_