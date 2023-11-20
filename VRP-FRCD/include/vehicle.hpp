#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include <iostream>
#include <random>


enum TYPE{
    RETAILER,
    SUPPLIER,
    OUTLETS
};

class Vehicle {
    private:
        TYPE type;
        static int glob_id;
        int id;
        const int COST;
        double capacity;
    public:
        Vehicle(TYPE type, int cost, double capacity) 
        : id(glob_id++), type(type), COST(cost), capacity(capacity) {};
        int getId();
        int getCost();
        double getCapacity();
        void setCapacity(double capacity);
        void print();
};

#endif // VEHICLE_HPP_