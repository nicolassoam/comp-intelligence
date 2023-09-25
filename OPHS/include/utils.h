#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

using trip_matrix = std::vector<std::vector<double>>;

namespace Util{
    using k_int = std::vector<int>;
    using k_double = std::vector<double>;
    using sstream = std::stringstream;
    using string = std::string;
    using return_t = std::pair<k_int,k_double>;

    /*
        Retorna informações sobre número de vértices, número de hotéis extras,
        número de viagens e tempo máximo de viagem no primeiro vetor
        enquato o segundo vetor contém o tempo de cada viagem.
    */
    return_t readInstance(std::string filename, trip_matrix& tour){
        int n_vertices, nextra_hotels, trips;
        double tmax;

        k_double t_lenght;
        k_int info(4);



        std::ifstream file;

        file.open(filename, std::ios::in);

        file >> n_vertices >> nextra_hotels >> trips >> tmax;

        info[0] = n_vertices;
        info[1] = nextra_hotels;
        info[2] = trips;
        info[3] = tmax;

        t_lenght.resize(trips);

        for(int i = 0; i < trips; i++){
            file >> t_lenght[i];
        }

        string line;
        getline(file, line);
        line = "";
        while(getline(file, line)){
            k_double aux;
            sstream ss(line);
            string value = " ";
            // std::cout << line << std::endl;
            while(ss >> value){
                if(strcmp(value.c_str(), "---------------------------------------------------") == 0)
                    continue;
                // std::cout << value << std::endl;
                aux.push_back(std::stod(value));
            }

            tour.push_back(aux);
        }

        file.close();
        
        return {info,t_lenght};
    };
}

#endif