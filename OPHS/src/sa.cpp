#include "../include/sa.hpp"

// constructor
SA::SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels) {
    // initialize random seed
    unsigned int seed = 42;
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

solution_t SA::insertIfFeasible(solution_t solution, tour_t& unvisitedLocations){
    
    //iterates over the trip to find the best location to insert
    double heuristic = 0.0;
    double bestHeuristic = 0.0;
    int bestLocation = 0;
    
    for(int trip = 0; trip < solution.size(); trip++){
        if(solution[trip].locations.size() <= 4)
            continue;
        std::uniform_int_distribution<int> dist(0, solution[trip].locations.size() - 3);
        int swapLocation = dist(rng);
            //checks if the length can be changed to add the new location to the trip between a (i,j) to (i, location) and (location, j)
        int from = solution[trip].locations[swapLocation];
        int to = solution[trip].locations[swapLocation + 1];
        int sucessor = solution[trip].locations[swapLocation + 2];

        for(int i = 0; i< unvisitedLocations.size();i++){
            heuristic = adjMatrix[from][unvisitedLocations[i]].score/ adjMatrix[from][unvisitedLocations[i]].dist;
            if(heuristic > bestHeuristic){
                bestHeuristic = heuristic;
                bestLocation = i;
            }
        }

        int addedLocation = unvisitedLocations[bestLocation];

        double prevLength = adjMatrix[from][to].dist;
        double newLength = adjMatrix[from][addedLocation].dist;
        double prevSucessorLength = adjMatrix[to][sucessor].dist;
        double sucessorLength = adjMatrix[addedLocation][sucessor].dist;

        // std::cout << "tripLength: " << solution[trip].tripLength << std::endl;
        if(solution[trip].tripLength + prevLength + prevSucessorLength >= newLength + sucessorLength){
            //delete the edge between from and to and add the edge between from and addedLocation and addedLocation and to
            unvisitedLocations.push_back(to);
            solution[trip].locations[swapLocation + 1] = addedLocation;
            solution[trip].tripLength += prevLength;
            solution[trip].tripLength += prevSucessorLength;
            solution[trip].tripLength -= newLength;
            solution[trip].tripLength -= sucessorLength;
            unvisitedLocations.erase(unvisitedLocations.begin() + bestLocation);
        }
    }
    return solution;
}

void SA::swapBetweenTrips(solution_t& solution){

    int trip1 = rand() % solution.size();
    int trip2 = rand() % solution.size();

    while(trip1 == trip2){
        trip2 = rand() % solution.size();
    }
    
    while(solution[trip1].locations.size() <= 3){
        trip1 = rand() % solution.size();
        
    }

    while(solution[trip2].locations.size() <= 3){
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

    std::swap(solution[trip1].locations[location1], solution[trip2].locations[location2]);
    std::swap(solution[trip1].locations[location1 + 1], solution[trip2].locations[location2 + 1]);
}


void SA::swapInTrip(solution_t& solution){
    int trip = rand() % solution.size();

    std::uniform_int_distribution<int> dist(1, solution[trip].locations.size() - 2);

    int location1 = dist(rng);
    int location2 = dist(rng);

    while(location1 == location2){
        location2 = dist(rng);
    }

    std::swap(solution[trip].locations[location1], solution[trip].locations[location2]);
}

Neighbor SA::generateNeighbor(solution_t currentSolution, tour_t& unvisitedLocations) {
    // select the movement
    Neighbor newSolution;
    newSolution.trips = currentSolution;
    swapBetweenTrips(newSolution.trips);
    newSolution.cost = objectiveFunction(newSolution.trips);
    
    if(newSolution.cost > this->currentSolution.cost){
        return newSolution;
    }

    newSolution.trips = currentSolution;
    swapBetweenTrips2(newSolution.trips);
    newSolution.cost = objectiveFunction(newSolution.trips);

    if(newSolution.cost > this->currentSolution.cost){
        return newSolution;
    }

    newSolution.trips = currentSolution;
    newSolution.trips = insertIfFeasible(currentSolution, unvisitedLocations);
    newSolution.cost = objectiveFunction(newSolution.trips);

    if(newSolution.cost > this->currentSolution.cost){
        return newSolution;
    }

    return newSolution;
}

void SA::printSolution() {
    double totalCost = 0.0;
    std::cout << "Best solution: " << bestSolution.cost << std::endl;
    std::cout << "Trips: " << std::endl;
    int t = 1;
    for (auto trip : bestSolution.trips) {
        double tripCost = 0.0;

        for (int i = 0; i < trip.locations.size() - 1; i++) {
            int from = trip.locations[i];
            int to = trip.locations[i + 1];
            tripCost += adjMatrix[from][to].score;
        }
        
        totalCost += tripCost;
        std::cout << "Trip cost: " << tripCost << std::endl;
        std::cout << "Trip " << t++;
        std::cout << " | Length: " << trip.tripLength << " | ";
        for (auto location : trip.locations) {
            std::cout << location << " ";
        }
        
        std::cout << std::endl;
    }
    std::cout << "Total Cost: " << totalCost << std::endl;
}

// implementation of simulated annealing algorithm
void SA::run(solution_t& initialSolution, tour_t& unvisitedLocations) {
    // initialize current solution

    this->currentSolution.trips = initialSolution;
    this->currentSolution.cost = this->objectiveFunction(initialSolution);
    // initialize best solution
    bestSolution = currentSolution;

    std::cout << "Optimizing solution..." << std::endl;

    std::cout << "Initial solution: " << currentSolution.cost << std::endl;

    // initialize temperature
    double temperature = this->initialTemperature;

    // loop until stopping criterion is met
    while (temperature > this->finalTemperature) {
        // loop for a certain number of iterations at each temperature
        for (int i = 0; i < this->iterationsPerTemperature; i++) {
            // generate a new candidate solution
            Neighbor candidateSolution = generateNeighbor(this->currentSolution.trips, unvisitedLocations);

            // calculate the acceptance probability
            double acceptanceProbability = std::exp((this->currentSolution.cost - candidateSolution.cost) / temperature);

            // determine whether to accept the candidate solution
            if (candidateSolution.cost > this->currentSolution.cost || acceptanceProbability > ((double) rand() / RAND_MAX)) {
                this->currentSolution = candidateSolution;
                this->currentSolution.cost = candidateSolution.cost;
            }

            // update best solution if necessary
            if (this->currentSolution.cost > this->bestSolution.cost) {
                this->bestSolution = this->currentSolution;
            }
        }

        // decrease the temperature
        temperature *= coolingFactor;
    }

    printSolution();
}
