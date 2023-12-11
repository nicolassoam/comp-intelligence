#ifndef NEIGHBORHOOD_HPP_
#define NEIGHBORHOOD_HPP_

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "vehicle.hpp"
#include "instance.hpp"

namespace Neighborhood
{   

    int getRandomPosition(int begin, int end) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(begin, end);
        return dis(gen);
    }

    // SHIFT 1-0: One node is transferred from one route to another route.
    void shift1_0(Vehicle& r1, Vehicle& r2, Instance* inst) {

        std::vector<int> r1Routes = r1.getRoutes();
        std::vector<int> r2Routes = r2.getRoutes();

        if (r1Routes.size() < 3 || r2Routes.size() < 3) return;
        
        int pos1 = getRandomPosition(1, r1.getRoutes().size() - 2);
        int pos2 = getRandomPosition(1, r2.getRoutes().size() - 2);

        r2Routes.insert(r2Routes.begin() + pos2, r1Routes[pos1]);
        r1Routes.erase(r1Routes.begin() + pos1);

        // verify if the routes are valid
        // check if the capacity is not exceeded

        double capacity = 0, newCapacity = 0;

        switch (r1.getType()) {
            case RETAILER:
                capacity = inst->demandPerRetailer[pos1-1].first;
                break;
            case OUTLETS:
                capacity = inst->outletDemand[pos1-1].first;
                break;
            case SUPPLIER:
                capacity = inst->demandPerProduct[pos1-1].first;
                break;
            default:
                break;
        }

        newCapacity = r2.getCapacity() - capacity;

        if (newCapacity < 0) return;

        r1.setRoutes(r1Routes);

        if (r1Routes.size() == 2) {
            r1.setCapacity(inst->capacity);
        } else {
            r1.setCapacity(r1.getCapacity()+capacity);
        }

        r2.setRoutes(r2Routes);
        r2.setCapacity(newCapacity);
    }

    // REINSERTION: One node is removed and later inserted into other position of the route.
    void reinsertion(Vehicle& r, Instance* inst) {
        std::vector<int> rRoutes = r.getRoutes();

        if (rRoutes.size() <= 3) return;
        
        int nodePos = getRandomPosition(1, r.getRoutes().size() - 2);
        int nodeId = rRoutes[nodePos];
        int newPos;
        do {
            newPos = getRandomPosition(1, r.getRoutes().size() - 2);
        } while (newPos == nodePos);

        rRoutes.erase(rRoutes.begin() + nodePos);
        rRoutes.insert(rRoutes.begin() + newPos, nodeId);

        r.setRoutes(rRoutes);
    }

    // 2-OPT: Two nonadjacent arcs are deleted and later other two are added in such a way that a new route is generated.
    void twoOpt(Vehicle& r, Instance* inst) {
        std::vector<int> rRoutes = r.getRoutes();

        // there must be at least four arcs to have two nonadjacent arcs
        if (rRoutes.size() < 5) return;
        
        // arcs can't be adjacent
        // arc 1 = pos1 -> pos1+1
        // arc 2 = pos2 -> pos2+1

        int pos1 = getRandomPosition(1, r.getRoutes().size() - 3);
        int pos2;
        do {
            pos2 = getRandomPosition(1, r.getRoutes().size() - 3);
        } while (pos2 == pos1);

        // pos1 must be smaller than pos2
        if (pos1 > pos2) {
            int aux = pos1;
            pos1 = pos2;
            pos2 = aux;
        }

        // reverse the arcs
        std::reverse(rRoutes.begin() + pos1, rRoutes.begin() + pos2 + 1);

        r.setRoutes(rRoutes);
    }

}

#endif // NEIGHBORHOOD_HPP_