#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include "types_inc.hpp"
#include "graph.hpp"


// Estrutura temporária de matriz de viagens


namespace Util{

    // Atalhos de tipos
    using k_int     = std::vector<int>;
    using sstream   = std::stringstream;
    using string    = std::string;
    using return_t  = std::pair<k_int,k_double>;

    double euclideanDistance(double x1, double y1, double x2, double y2){
        std::cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << std::endl;
        return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
    }

    /*
        Retorna informações sobre número de vértices, número de hotéis extras,
        número de viagens e tempo máximo de viagem no primeiro vetor
        enquanto o segundo vetor contém o tempo de cada viagem.
    */
    Graph* readInstance(std::string filename, matrix_t& tour){
        int n_vertices, nextra_hotels, trips;
        double tmax;

        k_double t_length;
        k_int info(4);

        std::ifstream file;

        file.open(filename, std::ios::in);

        file >> n_vertices >> nextra_hotels >> trips >> tmax;

        info[0] = n_vertices;
        info[1] = nextra_hotels;
        info[2] = trips;
        info[3] = tmax;

        t_length.resize(trips);

        for(int i = 0; i < trips; i++){
            file >> t_length[i];
        }

        string line;
        getline(file, line);
        getline(file, line);
        
        line = "";  

        int i = 0;

        while(getline(file, line)&& i < n_vertices + nextra_hotels){
            k_double aux;
            sstream ss(line);
            string value = " ";
            
            while(ss >> value ){
                if(strcmp(value.c_str(), "---------------------------------------------------") == 0){
                    break;
                }
                
                aux.push_back(std::stod(value));
                
            }
            
            tour.push_back(aux);
            i++;
        }

        // std::cout << tour[0][0] << std::endl;

        file.close();

        Graph *graph = new Graph(n_vertices, nextra_hotels, trips, tmax, t_length); 

        return graph;
    };
    
}

#endif