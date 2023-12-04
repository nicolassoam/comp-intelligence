#ifndef CUCKOO_HPP_
#define CUCKOO_HPP_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <numbers>  //defines phi


/*
    Modified cuckoo search algorithm for the VRP-FRCD problem
*/
//TODO: adapt the cuckoo struct to the VRP-FRCD problem
//TODO: adapt the MCS class to the VRP-FRCD problem
//TODO: adapt the solution_t type to the VRP-FRCD problem

using solution_t = std::vector<double>;
struct cuckoo{
    double  fitness;
    solution_t solution;
};

using nest_t = std::vector<cuckoo>;

class MCS{
    public:     MCS(int nNests, int nEggs, int nIterations,solution_t lowerBound, solution_t upperBound);
                double  fitness(solution_t solution);  
                void    search();
                void    printSolution();
                ~MCS();

    private:    double phi = std::numbers::phi;
                int nNests;
                solution_t lowerBound;
                solution_t upperBound;
                int nEggs;
                int nIterations;
                double maxLevyStepSize = 1;
                double levyStepSize;
                double pa = 0.25;
                nest_t nests;
                // choose a random cuckoo via levy flight
                cuckoo newCuckoo(int nest);
                void initPopulation();

};




#endif // CUCKOO_HPP_