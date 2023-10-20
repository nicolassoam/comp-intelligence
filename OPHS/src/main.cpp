#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/graph.hpp"
#include "../include/constructive.hpp"
#include "../include/sa.hpp"
#include <chrono>

int main(int argc, char const **argv)
{   
    std::string aux = argv[1];
    std::string filename = INPUT + aux;
    std::cout << filename << std::endl;
    
    matrix_t tour;

    // leitura da instância
    Graph* graph = Util::readInstance(filename, tour);
    graph->toGraphMatrix(tour);
    

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double> elapsed;
    double timeAvg = 0;
    double costAvg = 0;
    int executions = 10;
    double bestCost = 0;

    solution_t bestSolution;

    for(int i = 0; i < executions; i++){
        start = std::chrono::high_resolution_clock::now();
        Search::Constructive* constructive = new Search::Constructive(graph, 500);
        SA* sa = new SA(graph->getAdjMatrix(), 0.9999, 0.0001, 0.90, 5000, 2+graph->getNExtraHotels());
        //construção da solução inicial
        solution_t solution = constructive->greedySolution();
        // simulated annealing
        std::set<int> unused = constructive->getUnusedLocations();
        tour_t unusedLocations;
        unusedLocations.assign(unused.begin(), unused.end());
        
        sa->run(solution, unusedLocations);
        
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        // Util::saveSolution(aux, sa->getBestSolution(), graph,i+1, elapsed.count());

        timeAvg += elapsed.count();
        costAvg += sa->getBestCost();
        
        if(sa->getBestCost() > bestCost || i == 0){
            bestCost = sa->getBestCost();
            bestSolution = sa->getBestSolution();
        }

    }
    
    timeAvg /= executions;
    costAvg /= executions;

    Util::saveResult(aux, costAvg, timeAvg,bestCost, bestSolution);
    
    std::cout << "Tempo de execução: " << elapsed.count() << "s" << std::endl;
    return 0;
}