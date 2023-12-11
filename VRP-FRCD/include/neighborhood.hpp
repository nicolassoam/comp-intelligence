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
    void exchange (Vehicle &v);     // EXCHANGE 
    void swap2_1 (Vehicle &v1,Vehicle &v2, Instance* inst);       // SWAP(2,1)
    void cross (Vehicle &v1,Vehicle &v2, Instance* inst);        // CROSS

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

        std::cout << "Sizes: " << r1Routes.size() << " " << r2Routes.size() << std::endl;

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
    //Permutation of two customers being swapped
    void exchange(Vehicle &v){
        std::vector<int> routes = v.getRoutes();
        if(routes.size() < 4) return;
        int pos1 = getRandomPosition(1, routes.size()-2);
        int pos2 = getRandomPosition(1, routes.size()-2);

        //check if the positions are the same
        while(pos1 == pos2){
            pos2 = getRandomPosition(1, routes.size()-2);
        }

        // print routes before shift
        std::cout << "Before exchange: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < routes.size(); i++) {
            std::cout << routes[i] << " ";
        }
        std::cout << std::endl;
        std::swap(routes[pos1], routes[pos2]);

        std::cout << "after exchange: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < routes.size(); i++) {
            std::cout << routes[i] << " ";
        }
        std::cout << std::endl;
        v.setRoutes(routes);
    }

    /*Swap two adjacent customers from one route with one customer from
another route.*/
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
        newCapacity1 = v1.getCapacity() + d[r1[pos1+1]-1].first + d[r2[pos1]-1].first - d[r2[pos2]-1].first;
        //will gain one customer
        newCapacity2 = v2.getCapacity() + d[r2[pos2]-1].first - d[r1[pos1]-1].first - d[r1[pos1+1]-1].first;
        if(newCapacity1 < 0) return;
        if(newCapacity2 < 0) return;

        // print routes before shift
        std::cout << "Before swap: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.size(); i++) {
            std::cout << r1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.size(); i++) {
            std::cout << r2[i] << " ";
        }
        std::cout << std::endl;

        r2.insert(r2.begin()+pos2, r1[pos1+1]);
        std::swap(r1[pos1], r2[pos2]);
        r1.erase(r1.begin()+pos1+1);

        v1.setCapacity(newCapacity1);
        v2.setCapacity(newCapacity2);

        // print routes before shift
        std::cout << "after swap: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.size(); i++) {
            std::cout << r1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.size(); i++) {
            std::cout << r2[i] << " ";
        }
        std::cout << std::endl;
        
        v1.setRoutes(r1);
        v2.setRoutes(r2);
    }
    /*
        The arc between two adjacent customers i and j belonging to a route one
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

        newCapacity1 = v1.getCapacity() + d[r2[pos1+1]-1].first - d[r1[pos2+1]-1].first;
        newCapacity2 = v2.getCapacity() + d[r1[pos2+1]-1].first - d[r2[pos1+1]-1].first;

        // print routes before shift
        std::cout << "Before cross: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.size(); i++) {
            std::cout << r1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.size(); i++) {
            std::cout << r2[i] << " ";
        }
        std::cout << std::endl;

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

        // print routes before shift
        std::cout << "after cross: " << std::endl;
        std::cout << "r1: ";
        for (int i = 0; i < r1.size(); i++) {
            std::cout << r1[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "r2: ";
        for (int i = 0; i < r2.size(); i++) {
            std::cout << r2[i] << " ";
        }
        std::cout << std::endl;

        v1.setCapacity(newCapacity1);
        v2.setCapacity(newCapacity2);
    
    }

}

#endif // NEIGHBORHOOD_HPP_