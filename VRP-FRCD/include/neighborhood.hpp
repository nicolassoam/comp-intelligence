#ifndef NEIGHBORHOOD_HPP_
#define NEIGHBORHOOD_HPP_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "vehicle.hpp"

namespace Neighborhood
{   
    // SHIFT 1-0
    void shift1_0(Vehicle& r1, Vehicle& r2) {

        // print routes before shift
        std::cout << "Before shift: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.getRoutes().size(); i++) {
            std::cout << r1.getRoutes()[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.getRoutes().size(); i++) {
            std::cout << r2.getRoutes()[i] << " ";
        }
        std::cout << std::endl;

        int i = rand() % r1.getRoutes().size();
        int j = rand() % r2.getRoutes().size();

        if (r1.getRoutes().size() > 1 && r2.getRoutes().size() > 1 && i != j) {
            int node = r1.getRoutes()[i];
            r1.getRoutes().erase(r1.getRoutes().begin() + i);
            r2.getRoutes().insert(r2.getRoutes().begin() + j, node);
        }

        // print routes after shift
        std::cout << "After shift: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.getRoutes().size(); i++) {
            std::cout << r1.getRoutes()[i] << " ";
        }

        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.getRoutes().size(); i++) {
            std::cout << r2.getRoutes()[i] << " ";
        }
        std::cout << std::endl;
    }
}

#endif // NEIGHBORHOOD_HPP_