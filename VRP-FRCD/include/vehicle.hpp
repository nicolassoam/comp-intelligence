#ifndef VEHICLE_HPP_
#define VEHICLE_HPP_

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>

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
                TYPE getType();
                void setType(TYPE type);
                void setCapacity(double capacity);
                static std::vector<Vehicle> instantiateVehicles(int nVehicles, int COST, double capacity);
                void insertBetween(int i, int j, int k);
                std::vector<int> getRoutes();
                void addNode(int node);
                void shuffle();
                ~Vehicle();

    private:    TYPE type;
                int id;
                int COST;
                int coveredDist = 0;
                double capacity;
                static int globalId;
                std::vector<int> route;
};

#endif // VEHICLE_HPP_