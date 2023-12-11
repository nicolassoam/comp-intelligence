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
using v_int = std::vector<int>;

struct cuckoo{
    double      fitness;
    v           vehicles;
    int         usedVehicles = 0;
    v_int       vehicleTypes = {0,0,0};              // retailer, supplier, outlet
};

using nest_t = std::vector<cuckoo>;                  // population of cuckoos (one per nest)
using candidate = std::tuple<double,int,int,int>;    // heuristic, k, i, j
using list_t = std::vector<candidate>;               // list of candidates (heuristic, k, i, j)

class MCS{
    public:     MCS(int nNests, int nVehicles, int nIterations, Instance* inst); 
                void    search();
                void    printSolution();
                double  fitness(v vehicles, int usedVehicles);
                ~MCS();

    private:    double phi = std::numbers::phi;
                int nNests;
                Instance* inst;
                int nVehicles;
                int nIterations;
                double maxLevyStepSize = 2;
                double levyStepSize;
                double pa = 0.25;
                nest_t nests;

                // choose a random cuckoo via levy flight
                double levyFlight();
                cuckoo applyMovement(cuckoo c, std::vector<double>iteratorVector);
                void initPopulation();
                void supplierInit(cuckoo& cuckoo);
                void retailerInit(cuckoo& cuckoo);
                void outletInit(cuckoo& cuckoo);
};




#endif // CUCKOO_HPP_