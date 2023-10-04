#include "../include/graph.hpp"
#include "../include/types_inc.hpp"
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
            std::cout << j.dist << "\t";
        }
        std::cout << std::endl;
    }
}

void Graph::toGraphMatrix(matrix_t& tour){
       
        //  calculates the euclidean distance between hotels and stores it in a matrix
        for(int i = 0; i < tour.size(); i++){
            k_double &location = tour[i];

            std::vector<Node> aux;

            for(int j = 0; j < tour.size(); j++){
                k_double & other_location = tour[j];

                Node aux_;

                if(i == j){
                    aux_.dist = 0;
                    aux_.score = 0;
                    aux_.type = type_::NULLTYPE;

                    aux.push_back(aux_);
                }else{
                    aux_.dist = Graph::euclideanDistance(location[0], location[1], other_location[0], other_location[1]);
                    aux_.score = location[2];

                    if(aux_.score == 0) 
                        aux_.type = type_::HOTEL;
                    else 
                        aux_.type = type_::LOCAL;
                        
                    aux.push_back(aux_);
                }
            }
            this->adjMatrix.push_back(aux);
        }
    }

Graph::~Graph(){
    // std::cout << "Graph destructor called" << std::endl;
}