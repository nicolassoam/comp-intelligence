#include "../include/vehicle.hpp"

inline int Vehicle::getId() {
    return this->id;
}

inline int Vehicle::getCost() {
    return this->COST;
}

inline double Vehicle::getCapacity() {
    return this->capacity;
}

inline void Vehicle::setCapacity(double capacity) {
    this->capacity = capacity;
}


