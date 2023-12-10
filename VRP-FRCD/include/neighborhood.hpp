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

    int getRandomPosition(int begin, int end) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(begin, end);
        return dis(gen);
    }

    // SHIFT 1-0
    void shift1_0(Vehicle& r1, Vehicle& r2) {

        std::vector<int> r1Routes = r1.getRoutes();
        std::vector<int> r2Routes = r2.getRoutes();

        if (r1Routes.size() < 3 || r2Routes.size() < 3) return;

        // print routes before shift
        std::cout << "Before shift: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1Routes.size(); i++) {
            std::cout << r1Routes[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2Routes.size(); i++) {
            std::cout << r2Routes[i] << " ";
        }
        std::cout << std::endl;
        
        int pos1 = getRandomPosition(1, r1.getRoutes().size() - 2);
        int pos2 = getRandomPosition(1, r2.getRoutes().size() - 2);

        r2Routes.insert(r2Routes.begin() + pos2, r1Routes[pos1]);
        r1Routes.erase(r1Routes.begin() + pos1);

        // print routes after shift
        std::cout << "After shift: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1Routes.size(); i++) {
            std::cout << r1Routes[i] << " ";
        }

        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2Routes.size(); i++) {
            std::cout << r2Routes[i] << " ";
        }
        std::cout << std::endl;

        r1.setRoutes(r1Routes);
        r2.setRoutes(r2Routes);
    }
}

#endif // NEIGHBORHOOD_HPP_