#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include <iostream>
#include <fstream>
#include <random>


enum TYPE{
    RETAILER,
    SUPPLIER,
    OUTLETS
};

class Vehicle {
    public:
        Vehicle(TYPE type, int cost, double capacity) 
        : id(globalId++), type(type), COST(cost), capacity(capacity) {};
        int getId();
        int getCost();
        double getCapacity();
        void setCapacity(double capacity);
        std::ostream& operator<<(std::ostream& os);

    private:
        TYPE type;
        static int globalId;
        int id;
        const int COST;
        double capacity;
};

#endif // VEHICLE_HPP_