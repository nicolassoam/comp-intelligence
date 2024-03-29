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
    using av = std::vector<std::pair<double,availability>>;
    int getRandomPosition(int begin, int end);

    void shift1_0(Vehicle& r1, Vehicle& r2, Instance* inst);        // SHIFT(1,0)
    void reinsertion(Vehicle& r, Instance* inst);                   // REINSERTION
    void twoOpt(Vehicle& r, Instance* inst);                        // 2-OPT
    void exchange (Vehicle &v);                                     // EXCHANGE 
    void swap2_1 (Vehicle &v1,Vehicle &v2, Instance* inst);         // SWAP(2,1)
    void cross (Vehicle &v1,Vehicle &v2, Instance* inst);           // CROSS

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

        

        // verify if the routes are valid
        // check if the capacity is not exceeded

        double capacity = 0, newCapacity = 0;

        switch (r1.getType()) {
            case RETAILER:
                capacity = inst->demandPerRetailer[pos1].first;
                break;
            case OUTLETS:
                capacity = inst->outletDemand[pos1].first;
                break;
            case SUPPLIER:
                capacity = inst->demandPerProduct[pos1].first;
                break;
            default:
                break;
        }
        r2Routes.insert(r2Routes.begin() + pos2, r1Routes[pos1]);
        r1Routes.erase(r1Routes.begin() + pos1);
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

    //EXCHANGE: Permutation of two customers being swapped
    void exchange(Vehicle &v){
        std::vector<int> routes = v.getRoutes();
        if(routes.size() < 4) return;
        int pos1 = getRandomPosition(1, routes.size()-2);
        int pos2 = getRandomPosition(1, routes.size()-2);

        //check if the positions are the same
        while(pos1 == pos2){
            pos2 = getRandomPosition(1, routes.size()-2);
        }

        
        std::swap(routes[pos1], routes[pos2]);

        v.setRoutes(routes);
    }

    // SWAP 2-1: Swap two adjacent customers from one route with one customer from another route.
    void swap2_1(Vehicle &v1, Vehicle &v2, Instance* inst){
        std::vector<int> r1 = v1.getRoutes();
        std::vector<int> r2 = v2.getRoutes();
        if(r1.size() < 4 || r2.size() < 3) return;
        int pos1 = getRandomPosition(1, r1.size()-3);
        int pos2 = getRandomPosition(1, r2.size()-2);
        
        //new capacity
        double newCapacity1 = 0;
        double newCapacity2 = 0;
        av d;
        switch(v1.getType()){
            case RETAILER:
                d = inst->demandPerRetailer;
                break;
            case SUPPLIER:
                d = inst->demandPerProduct;
                break;
            case OUTLETS:
                d = inst->outletDemand;
                break;
            default:
                break;
        }
        
        //will lose one customer
        newCapacity1 = v1.getCapacity() + d[pos1+1].first + d[pos1].first - d[pos2].first;
        //will gain one customer
        newCapacity2 = v2.getCapacity() + d[pos2].first - d[pos1].first - d[pos1+1].first;

        std::cout << "newCapacity2: " << newCapacity2 << std::endl;
        std::cout << "newCapacity1: " << newCapacity1 << std::endl;

        if(newCapacity1 < 0) return;
        if(newCapacity2 < 0) return;
        
        // print routes before shift
    
        r2.insert(r2.begin()+pos2, r1[pos1+1]);
        std::swap(r1[pos1], r2[pos2]);
        r1.erase(r1.begin()+pos1+1);
        
        v1.setCapacity(newCapacity1);
        v2.setCapacity(newCapacity2);


        
        v1.setRoutes(r1);
        v2.setRoutes(r2);
    }

    /*  CROSS: The arc between two adjacent customers i and j belonging to a route one
        and the arc between two adjacent customers i′ and j′ belonging to route two
        are both removed. Next an arc is inserted connecting i and j′ and another is
        inserted linking i′ and j   
    */
    void cross(Vehicle &v1, Vehicle &v2, Instance* inst){
        std::vector<int> r1 = v1.getRoutes();
        std::vector<int> r2 = v2.getRoutes();

        if(r1.size() < 4 || r2.size() < 4) return;

        int pos1 = getRandomPosition(1, r1.size()-3);
        int pos2 = getRandomPosition(1, r2.size()-3);

        //new capacity
        double newCapacity1 = 0;
        double newCapacity2 = 0;
        av d;
        switch(v1.getType()){
            case RETAILER:
                d = inst->demandPerRetailer;
                break;
            case SUPPLIER:
                d = inst->demandPerProduct;
                break;
            case OUTLETS:
                d = inst->outletDemand;
                break;
            default:
                break;
        }

        newCapacity1 = v1.getCapacity() + d[pos1+1].first - d[pos2+1].first;
        newCapacity2 = v2.getCapacity() + d[pos2+1].first - d[pos1+1].first;


        if(newCapacity1 < 0) return;
        if(newCapacity2 < 0) return;
        //i
        // pos1
        //j
        // pos1+1
        // i aresta para j
        //i'
        // pos2
        //j'
        // pos2+1
        // i' aresta para j'
        
        //remove arco i pra j, i' pra j', cria arco i pra j', i' pra j

        std::swap(r1[pos1+1], r2[pos2+1]);


        v1.setCapacity(newCapacity1);
        v2.setCapacity(newCapacity2);
    
    }
}

#endif // NEIGHBORHOOD_HPP_