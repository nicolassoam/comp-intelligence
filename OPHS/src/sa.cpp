#include "../include/sa.hpp"

// constructor
SA::SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels) {
    // initialize random seed
    srand(time(NULL));
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

    int location1 = rand() % solution[trip1].locations.size();
    int location2 = rand() % solution[trip2].locations.size();

    //cant swap if a hotel is selected
    for(int i = 0; i < this->nHotels; i++){
        if(solution[trip1].locations[location1] == i){
            location1 = rand() % solution[trip1].locations.size();
            i = 0;
        }

        if(solution[trip2].locations[location2] == i){
            location2 = rand() % solution[trip2].locations.size();
            i = 0;
        }
    }

    int aux = solution[trip1].locations[location1];
    solution[trip1].locations[location1] = solution[trip2].locations[location2];
    solution[trip2].locations[location2] = aux;
}

void SA::swapInTrip(solution_t& solution){
    int trip = rand() % solution.size();

    int location1 = rand() % solution[trip].locations.size();
    int location2 = rand() % solution[trip].locations.size();

    //cant swap if a hotel is selected
    for(int i = 0; i < this->nHotels; i++){
        if(solution[trip].locations[location1] == i){
            location1 = rand() % solution[trip].locations.size();
            i = 0;
        }

        if(solution[trip].locations[location2] == i){
            location2 = rand() % solution[trip].locations.size();
            i = 0;
        }
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
        swapInTrip(currentSolution);
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
}

// implementation of simulated annealing algorithm
void SA::run(solution_t& initialSolution) {
    // initialize current solution

    this->currentSolution.trips = initialSolution;
    this->currentSolution.fitness = this->objectiveFunction(initialSolution);
    // initialize best solution
    bestSolution = currentSolution;

    // initialize temperature
    double temperature = initialTemperature;

    // loop until stopping criterion is met
    while (temperature > finalTemperature) {
        // loop for a certain number of iterations at each temperature
        for (int i = 0; i < iterationsPerTemperature; i++) {
            // generate a new candidate solution
            Neighbor candidateSolution = generateNeighbor(currentSolution.trips);

            // calculate the cost of the candidate solution
            double candidateFitness = this->objectiveFunction(candidateSolution.trips);

            // calculate the acceptance probability
            double acceptanceProbability = exp((currentSolution.fitness - candidateFitness) / temperature);

            // determine whether to accept the candidate solution
            if (candidateFitness < currentSolution.fitness || acceptanceProbability > ((double) rand() / RAND_MAX)) {
                currentSolution = candidateSolution;
                currentSolution.fitness = candidateFitness;
            }

            // update best solution if necessary
            if (currentSolution.fitness < bestSolution.fitness) {
                bestSolution = currentSolution;
                bestSolution.fitness = currentSolution.fitness;
            }
        }

        // decrease the temperature
        temperature *= coolingFactor;
    }

    printSolution();
}
