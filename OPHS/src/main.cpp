#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"
#include "../include/sa.hpp"

int main(int argc, char const **argv)
{   
    std::string aux = "SET5-15-10/100-150-15-10.ophs";
    std::string filename = INPUT + aux;
    std::cout << filename << std::endl;
    
    matrix_t tour;

    // leitura da instância
    Graph* graph = Util::readInstance(filename, tour);
    graph->toGraphMatrix(tour);

    //construção da solução inicial
    Search::Constructive* constructive = new Search::Constructive(graph, 500);
    solution_t solution = constructive->greedySolution();
    Util::saveSolution(aux, solution, graph);

    // simulated annealing
    std::set<int> unused = constructive->getUnusedLocations();
    tour_t unusedLocations;
    unusedLocations.assign(unused.begin(), unused.end());

    SA* sa = new SA(graph->getAdjMatrix(), 0.9999, 0.0001, 0.90, 3000, 2+graph->getNExtraHotels());
    sa->run(solution, unusedLocations);
    Util::saveSolution(aux, sa->getBestSolution(), graph);
    
    return 0;
}