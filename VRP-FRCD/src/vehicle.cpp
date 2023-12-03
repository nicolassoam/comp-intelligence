#include "../include/vehicle.hpp"

int Vehicle::globalId = 0;

Vehicle::Vehicle(TYPE type, int COST, double capacity) {
    this->type = type;
    this->id = Vehicle::globalId++;
    this->COST = COST;
    this->capacity = capacity;
}

int Vehicle::getId() {
    return this->id;
}

int Vehicle::getCost() {
    return this->COST;
}

double Vehicle::getCapacity() {
    return this->capacity;
}

void Vehicle::setCapacity(double capacity) {
    this->capacity = capacity;
}

void Vehicle::setType(TYPE type) {
    this->type = type;
}

std::vector<Vehicle> Vehicle::instantiateVehicles(int nVehicles, int COST, double capacity) {
    std::vector<Vehicle> vehicles;
    for (int i = 0; i < nVehicles; i++) {
        vehicles.push_back(Vehicle(VEHICLE, COST, capacity));
    }
    return vehicles;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
    os << "Vehicle: " << v.id << std::endl;
    os << "COST: " << v.COST << std::endl;
    os << "capacity: " << v.capacity;
    return os;
}


