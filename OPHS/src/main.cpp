#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"
#include "../include/sa.hpp"

int main(int argc, char const **argv)
{   
    std::string aux = "SET1 3-4/100-40-3-4.ophs";
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
    
    solution_t solution = constructive->greedySolution();

    // graph->printGraph();

    SA* sa = new SA(graph->getAdjMatrix(), 0.9999, 0.0001, 0.90, 3000, 2+ graph->getNExtraHotels());
    sa->run(solution);
    return 0;
}