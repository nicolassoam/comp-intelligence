#ifndef NEIGHBORHOOD_HPP_
#define NEIGHBORHOOD_HPP_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "cuckoo.hpp"
#include "instance.hpp"

namespace Neighborhood
{
    void shift10 (Vehicle &v1,Vehicle &v2, Instance* inst);      // SHIFT(1,0)
    void reinsertion (Vehicle &v, Instance* inst);  // REINSERTION
    void twoOpt (Vehicle &v, Instance* inst);       // 2-OPT
    void exchange (Vehicle &v, Instance* inst);     // EXCHANGE 
    void swap21 (Vehicle &v1,Vehicle &v2, Instance* inst);       // SWAP(1,2)
    void cross (Vehicle &v1,Vehicle &v2, Instance* inst);        // CROSS
}

#endif // NEIGHBORHOOD_HPP_