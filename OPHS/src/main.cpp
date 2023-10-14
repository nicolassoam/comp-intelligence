#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"


int main(int argc, char const **argv)
{   
    std::string aux = "SET1 1-2/64-45-1-2.ophs";
    std::string filename = INPUT + aux;
    std::cout << filename << std::endl;
    
    matrix_t tour;

    Graph* graph = Util::readInstance(filename, tour);
    graph->toGraphMatrix(tour);

    std::cout << "n_vertices: " << graph->getNVertices() << std::endl;
    std::cout << "nextra_hotels: " << graph->getNExtraHotels() << std::endl;
    std::cout << "trips: " << graph->getNumTrips() << std::endl;
    std::cout << "tmax: " << graph->getTourLength() << std::endl;
    std::cout << "t_length: " << std::endl;

    for(auto i : graph->getTripLenghts()){
        std::cout << i << " ";
    }

    std::cout << std::endl;
    std::cout << "tour: " << std::endl;

    trip_matrix adjMatrix = graph->getAdjMatrix();
    
    for(auto i : tour){
        for(auto j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    } 
      
    return 0;
}