#include "../include/vehicle.hpp"

int Vehicle::globalId = 0;

Vehicle::Vehicle(TYPE type, int COST, double capacity) {
    this->type = type;
    this->id = Vehicle::globalId++;
    this->COST = COST;
    this->capacity = capacity;
    this->route = std::vector<int>(2, 0);
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

void Vehicle::insertBetween(int i, int j, int k) {
    std::vector<int>::iterator it = std::find(this->route.begin(), this->route.end(), i);

    this->route.insert(it + 1, k);
}

std::vector<int> Vehicle::getRoutes() {
    return this->route;
}

void Vehicle::addNode(int node) {
    this->route.push_back(node);
}

TYPE Vehicle::getType() {
    return this->type;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
    os << "Vehicle: " << v.id << std::endl;
    os << "id: " << v.id << std::endl;
    os << "type: " << v.type << std::endl;
    os << "COST: " << v.COST << std::endl;
    os << "capacity: " << v.capacity << std::endl;
    os << "route: ";
    for (int i = 0; i < v.route.size(); i++) {
        os << v.route[i] << " ";
    }
    return os;
}

Vehicle::~Vehicle() {
    route.clear();
}


