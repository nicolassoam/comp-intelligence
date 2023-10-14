#include "../include/sa.hpp"

// constructor
SA::SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels) {
    // initialize random seed
    unsigned int seed = 40;
    srand(seed);

    this->rng = rand_gen(seed);

    this->initialTemperature = initialTemperature;
    this->finalTemperature = finalTemperature;
    this->coolingFactor = coolingFactor;
    this->iterationsPerTemperature = iterationsPerTemperature;
    this->nHotels = nHotels;
    this->adjMatrix = adjMatrix;
}

double SA::objectiveFunction(solution_t& solution) {
    double totalCost = 0.0;

    // iterate over all trips in the solution
    for (int i = 0; i < solution.size(); i++) {
        // calculate the cost of the current trip
        double tripCost = 0.0;
        for (int j = 0; j < solution[i].locations.size() - 1; j++) {
            int from = solution[i].locations[j];
            int to = solution[i].locations[j + 1];
            tripCost += adjMatrix[from][to].score;
        }
        // add the cost of the current trip to the total cost
        totalCost += tripCost;
    }

    return totalCost;
}

void SA::swapBetweenTrips(solution_t& solution){

    int trip1 = rand() % solution.size();
    int trip2 = rand() % solution.size();

    while(trip1 == trip2){
        trip2 = rand() % solution.size();
    }
    
    std::uniform_int_distribution<int> dist(1, solution[trip1].locations.size() - 2);
    int location1 = dist(rng);

    dist = std::uniform_int_distribution<int>(1, solution[trip2].locations.size() - 2);
    int location2 = dist(rng);

    int aux = solution[trip1].locations[location1];
    solution[trip1].locations[location1] = solution[trip2].locations[location2];
    solution[trip2].locations[location2] = aux;
}

void SA::swapBetweenTrips2(solution_t& solution){

    int trip1 = rand() % solution.size();
    int trip2 = rand() % solution.size();

    while(trip1 == trip2){
        trip2 = rand() % solution.size();
    }

    while(solution[trip1].locations.size() <= 3 || solution[trip2].locations.size() <=3){
        trip1 = rand() % solution.size();
        trip2 = rand() % solution.size();
    }

    std::uniform_int_distribution<int> dist(1, solution[trip1].locations.size() - 2);
    int location1 = dist(rng);

    dist = std::uniform_int_distribution<int>(1, solution[trip2].locations.size() - 2);
    int location2 = dist(rng);

    while(solution[trip1].locations[location1 + 1] == solution[trip1].locations.back())
        location1--;
    
    while(solution[trip2].locations[location2 + 1] == solution[trip2].locations.back())
        location2--;

    int aux = solution[trip1].locations[location1];
    solution[trip1].locations[location1] = solution[trip2].locations[location2];
    solution[trip2].locations[location2] = aux;

    aux = solution[trip1].locations[location1 + 1];
    solution[trip1].locations[location1 + 1] = solution[trip2].locations[location2 + 1];
    solution[trip2].locations[location2 + 1] = aux;
}


void SA::swapInTrip(solution_t& solution){
    int trip = rand() % solution.size();

    std::uniform_int_distribution<int> dist(1, solution[trip].locations.size() - 2);

    int location1 = dist(rng);
    int location2 = dist(rng);

    while(location1 == location2){
        location2 = dist(rng);
    }

    int aux = solution[trip].locations[location1];
    solution[trip].locations[location1] = solution[trip].locations[location2];
    solution[trip].locations[location2] = aux;
}

Neighbor SA::generateNeighbor(solution_t currentSolution) {
    // select the movement
    bool swapBetween = rand() % 2;
    
    if(swapBetween){
        swapBetweenTrips(currentSolution);
    } else {
        swapBetweenTrips2(currentSolution);
    }

    // create and return the new neighbor
    Neighbor neighbor;
    neighbor.trips = currentSolution;
    neighbor.fitness = objectiveFunction(currentSolution);
    return neighbor;
}

void SA::printSolution() {
    double totalCost = 0.0;
    std::cout << "Best solution: " << bestSolution.fitness << std::endl;
    std::cout << "Trips: " << std::endl;
    for (auto trip : bestSolution.trips) {
        double tripCost = 0.0;

        for (int i = 0; i < trip.locations.size() - 1; i++) {
            int from = trip.locations[i];
            int to = trip.locations[i + 1];
            tripCost += adjMatrix[from][to].score;
        }
        
        totalCost += tripCost;
        std::cout << "Trip cost: " << tripCost << std::endl;
        std::cout << "Trip: ";
        for (auto location : trip.locations) {
            std::cout << location << " ";
        }
        
        std::cout << std::endl;
    }
    std::cout << "Total Cost: " << totalCost << std::endl;
}

// implementation of simulated annealing algorithm
void SA::run(solution_t& initialSolution) {
    // initialize current solution

    this->currentSolution.trips = initialSolution;
    this->currentSolution.fitness = this->objectiveFunction(initialSolution);
    // initialize best solution
    bestSolution = currentSolution;

    std::cout << "Optimizing solution..." << std::endl;

    std::cout << "Initial solution: " << currentSolution.fitness << std::endl;

    // initialize temperature
    double temperature = this->initialTemperature;

    // loop until stopping criterion is met
    while (temperature > this->finalTemperature) {
        // loop for a certain number of iterations at each temperature
        for (int i = 0; i < this->iterationsPerTemperature; i++) {
            // generate a new candidate solution
            Neighbor candidateSolution = generateNeighbor(this->currentSolution.trips);

            // calculate the acceptance probability
            double acceptanceProbability = std::exp((this->currentSolution.fitness - candidateSolution.fitness) / temperature);

            // determine whether to accept the candidate solution
            if (candidateSolution.fitness < this->currentSolution.fitness || acceptanceProbability > ((double) rand() / RAND_MAX)) {
                this->currentSolution = candidateSolution;
                this->currentSolution.fitness = candidateSolution.fitness;
            }

            // update best solution if necessary
            if (this->currentSolution.fitness < this->bestSolution.fitness) {
                this->bestSolution = this->currentSolution;
            }
        }

        // decrease the temperature
        temperature *= coolingFactor;
    }

    printSolution();
}
