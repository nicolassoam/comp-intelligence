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
    void shift10 (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);      // SHIFT(1,0)
    void reinsertion (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);  // REINSERTION
    void twoOpt (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);       // 2-OPT
    void exchange (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);     // EXCHANGE 
    void swap12 (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);       // SWAP(1,2)
    void cross (cuckoo& oldEgg, cuckoo& newEgg, Instance* inst);        // CROSS
}

#endif // NEIGHBORHOOD_HPP_