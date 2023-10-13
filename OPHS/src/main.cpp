#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"

int main(int argc, char const **argv)
{   
    std::string aux = "SET1 2-3/102-60-2-3.ophs";
    std::string filename = INPUT + aux;
    std::cout << filename << std::endl;
    
    matrix_t tour;
 
    Graph* graph = Util::readInstance(filename, tour);
    
    graph->toGraphMatrix(tour);
    // for(auto i : tour){
    //     for(auto j : i){
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }

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
    // graph->printGraph();

    Search::Constructive* constructive = new Search::Constructive(graph, 200);
    
    auto solution = constructive->greedySolution();

    std::cout << "solution: " << std::endl;
    for(auto i : solution){
        std::cout <<"trip: " << i.tripLength << std::endl; 
        for(auto j : i.locations){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}