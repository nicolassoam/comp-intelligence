#ifndef TYPES_H_
#define TYPES_H_
#include <iostream>
#include <vector>
#include <string>

struct Node{
    double dist;
    int score;
};

using trip_matrix   = std::vector<std::vector<Node>>;
using matrix_t      = std::vector<std::vector<double>>;
using k_double      = std::vector<double>;
using tour_t        = std::vector<int>;



#endif // TYPES_H_