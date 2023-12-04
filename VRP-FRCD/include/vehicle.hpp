#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include <iostream>
#include <fstream>
#include <random>
#include <vector>

enum TYPE{
    RETAILER,
    SUPPLIER,
    OUTLETS,
    VEHICLE
};

class Vehicle {
    public:     friend std::ostream& operator<<(std::ostream& os, const Vehicle& v);
                Vehicle(TYPE type, int cost, double capacity);
                int getId();
                int getCost();
                double getCapacity();
                void setType(TYPE type);
                void setCapacity(double capacity);
                static std::vector<Vehicle> instantiateVehicles(int nVehicles, int COST, double capacity);
                ~Vehicle();

    private:    TYPE type;
                int id;
                int COST;
                double capacity;
                static int globalId;
                std::vector<int> route;
};

#endif // VEHICLE_HPP_