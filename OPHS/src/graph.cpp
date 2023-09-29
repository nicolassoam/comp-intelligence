#include "../include/graph.hpp"

Graph::Graph(int nVertices, int nExtraHotels, int numTrips, double tourLength, k_double tripLenghts, trip_matrix tour){
    this->nVertices = nVertices;
    this->nExtraHotels = nExtraHotels;
    this->numTrips = numTrips;
    this->tripLenghts = tripLenghts;
    this->tourLength = tourLength;
    this->tour = tour;
}

int Graph::getNVertices(){
    return this->nVertices;
}

int Graph::getNExtraHotels(){
    return this->nExtraHotels;
}

int Graph::getNumTrips(){
    return this->numTrips;
}

double Graph::getTourLength(){
    return this->tourLength;
}

k_double Graph::getTripLenghts(){
    return this->tripLenghts;
}

trip_matrix Graph::getTour(){
    return this->tour;
}

Graph::~Graph(){
    // std::cout << "Graph destructor called" << std::endl;
}