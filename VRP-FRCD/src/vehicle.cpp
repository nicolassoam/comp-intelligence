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

void Vehicle::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    if(this->route.size() > 3)
        std::shuffle(this->route.begin()+1, this->route.end()-1, g);
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
    os << "----------" << std::endl;
    os << "Vehicle: " << v.id << std::endl;
    os << "id: " << v.id << std::endl;
    os << "type: ";
    
    switch (v.type) {
        case VEHICLE:
            os << "VEHICLE"<< std::endl;
            break;
        case SUPPLIER:
            os << "SUPPLIER" << std::endl;
            break;
        case RETAILER:
            os << "RETAILER" << std::endl;
            break;
        case OUTLETS:
            os << "OUTLET" << std::endl;
            break;
    }
    
    os << "COST: " << v.COST << std::endl;
    os << "capacity: " << v.capacity << std::endl;
    os << "route: ";
    for (int i = 0; i < v.route.size(); i++) {
        os << v.route[i] << " ";
    }
    os << std::endl;
    os << "----------" << std::endl;
    return os;
}

Vehicle::~Vehicle() {
    route.clear();
}


