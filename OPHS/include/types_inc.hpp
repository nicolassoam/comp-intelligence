#ifndef TYPES_H_
#define TYPES_H_
#include <iostream>
#include <vector>
#include <string>
#include <random>

enum type_{
    HOTEL,
    LOCAL,
    NULLTYPE = -1
};
struct Node{
    double dist;
    int score;
    type_ type;
};


using tour_t        = std::vector<int>;

struct Trip{
    tour_t  locations;
    double  tripLength;
    bool    finished = false;
};


using trip_matrix   = std::vector<std::vector<Node>>;
using matrix_t      = std::vector<std::vector<double>>;
using k_double      = std::vector<double>;
using solution_t    = std::vector<Trip>;

struct Neighbor{
    std::vector<Trip> trips;
    double cost;
};


static const unsigned int BIG = 99999;


#endif // TYPES_H_