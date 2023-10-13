#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"

int main(int argc, char const **argv)
{   
    std::string aux = "SET5 10-6/66-129-10-6.ophs";
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
    
    
    Search::Constructive* constructive = new Search::Constructive(graph, 500);
    
    auto solution = constructive->greedySolution();

    // graph->printGraph();

    return 0;
}