#ifndef TYPES_H_
#define TYPES_H_
#include <iostream>
#include <vector>
#include <string>

// the first value of the pair is the distance and the second is the score
using trip_matrix   = std::vector<std::vector<std::pair<double,int>>>;
using matrix_t        = std::vector<std::vector<double>>;
using k_double      = std::vector<double>;
using tour_t        = std::vector<int>;
#endif // TYPES_H_