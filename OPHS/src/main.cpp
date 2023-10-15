#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"
#include "../include/sa.hpp"

int main(int argc, char const **argv)
{   
    std::string aux = "SET5-10-6/66-129-10-6.ophs";
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
    std::set<int> unused;
    tour_t fullTour;
    for(auto i : solution){
        for(auto j : i.locations){
            fullTour.push_back(j);
        }
    }
    
    for(int i = 2 + graph->getNExtraHotels(); i < graph->getNVertices() + graph->getNExtraHotels(); i++){
        if(std::find(fullTour.begin(), fullTour.end(), i) == fullTour.end()){
            unused.insert(i);
        }
    }

    std::cout << "unused locations: " << std::endl;
    for(auto i : unused){
        std::cout << i << " ";
    }

    tour_t unusedLocations;
    unusedLocations.assign(unused.begin(), unused.end());
    

    // graph->printGraph();

    SA* sa = new SA(graph->getAdjMatrix(), 0.9999, 0.0001, 0.90, 3000, 2+ graph->getNExtraHotels());

    sa->run(solution, unusedLocations);
    
    Util::saveSolution(aux, solution, graph);
    return 0;
}