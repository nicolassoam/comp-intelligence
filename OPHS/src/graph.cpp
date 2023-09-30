#include "../include/graph.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

Graph::Graph(int nVertices, int nExtraHotels, int numTrips, double tourLength, k_double tripLenghts){
    this->nVertices = nVertices;
    this->nExtraHotels = nExtraHotels;
    this->numTrips = numTrips;
    this->tripLenghts = tripLenghts;
    this->tourLength = tourLength;
    this->adjMatrix.resize(nVertices + nExtraHotels);
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

trip_matrix Graph::getAdjMatrix(){
    return this->adjMatrix;
}

double Graph::euclideanDistance(double x1, double y1, double x2, double y2){
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

void Graph::printGraph(){
    for(auto i : this->adjMatrix){
        for(auto j : i){
            std::cout << j.first << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::toGraphMatrix(matrix_t& tour){
       
        //  calculates the euclidean distance between hotels and stores it in a matrix
        for(int i = 0; i < tour.size(); i++){
            auto &hotel = tour[i];
            std::vector<std::pair<double,int>> aux;
            for(int j = 0; j < tour.size(); j++){
                auto & other_hotel = tour[j];
                if(i == j){
                    aux.push_back(std::make_pair(0,0));
                }else{
                    aux.push_back(std::make_pair(Graph::euclideanDistance(hotel[0], hotel[1], other_hotel[0], other_hotel[1]), hotel[2]));
                }
                this->adjMatrix.push_back(aux);
            }
            
        }
    }

Graph::~Graph(){
    // std::cout << "Graph destructor called" << std::endl;
}