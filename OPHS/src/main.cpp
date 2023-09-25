#include "../include/utils.h"
#include "../include/config.h"
#include "../include/graph.h"


int main(int argc, char const **argv)
{   
    std::string aux = "SET1 1-2/64-45-1-2.ophs";
    std::string filename = INPUT + aux;
    std::cout << filename << std::endl;
    
    trip_matrix tour;

    auto [info, t_lenght] = Util::readInstance(filename, tour);

    std::cout << "n_vertices: " << info[0] << std::endl;
    std::cout << "nextra_hotels: " << info[1] << std::endl;
    std::cout << "trips: " << info[2] << std::endl;
    std::cout << "tmax: " << info[3] << std::endl;
    std::cout << "t_lenght: " << std::endl;
    for(auto i : t_lenght){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "tour: " << std::endl;
    for(auto i : tour){
        for(auto j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }   
    return 0;
}