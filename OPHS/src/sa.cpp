#include "../include/sa.hpp"

// constructor
SA::SA(trip_matrix adjMatrix,double initialTemperature, double finalTemperature, double coolingFactor, int iterationsPerTemperature, int nHotels) {
    // initialize random seed

    std::cout << "-------------------" << std::endl;
    std::cout << "Simulated Annealing" << std::endl;

    unsigned int seed = time(0);
    std::cout << "Seed: " << seed << std::endl;
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
    double tourScore = 0.0;

    // iterate over all trips in the solution
    for (int i = 0; i < solution.size(); i++) {
        // calculate the cost of the current trip
        double tripScore = 0.0;
        for (int j = 0; j < solution[i].locations.size() - 1; j++) {
            int from = solution[i].locations[j];
            int to = solution[i].locations[j + 1];
            tripScore += adjMatrix[from][to].score;
        }
        // add the score of current trip to total score
        tourScore += tripScore;
    }

    return tourScore;
}

void SA::insertIfFits(solution_t& solution, tour_t& unvisitedLocations){
    if(unvisitedLocations.size() == 0)
        return;
    
    int tries = 0;
    while(tries < 500){
        for(int trip = 0; trip < solution.size(); trip++){
            for(int location = 0; location < solution[trip].locations.size()-1; location++){
                int from = solution[trip].locations[location];
                int to = solution[trip].locations[location + 1];

                for(int added = 0; added < unvisitedLocations.size(); added++){
                    int sucessor = unvisitedLocations[added];
                    double prevLength = adjMatrix[from][to].dist;
                    double newLength = adjMatrix[from][sucessor].dist + adjMatrix[sucessor][to].dist;

                    if(solution[trip].tripLength + prevLength >= newLength){
                        //delete the edge between from and to and add the edge between from and addedLocation and addedLocation and to
                        solution[trip].locations.insert(solution[trip].locations.begin() + location + 1, sucessor);
                        solution[trip].tripLength += prevLength;
                        solution[trip].tripLength -= newLength;
                        unvisitedLocations.erase(unvisitedLocations.begin() + added);
                    }
                }

            }
        }
        tries++;
    }
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

    // escolhe duas trips aleatoriamente 

    int trip1 = rand() % solution.size();
    int trip2 = rand() % solution.size();

    while(solution[trip1].locations.size() <= 3){
        trip1 = rand() % solution.size();
    }

    while(trip1 == trip2 || solution[trip2].locations.size() <= 3){
        trip2 = rand() % solution.size();
    }

    // escolhe duas localidades aleatoriamente, exceto os hotéis

    std::uniform_int_distribution<int> dist(1, solution[trip1].locations.size() - 2);
    int location1 = dist(rng);

    dist = std::uniform_int_distribution<int>(1, solution[trip2].locations.size() - 2);
    int location2 = dist(rng);

    //std::cout << "trocando " << solution[trip1].locations[location1] << " com " << solution[trip2].locations[location2] << std::endl;

    // calcula distância percorrida no segmento anterior na trip 1
    double prevLength1 = adjMatrix[solution[trip1].locations[location1 - 1]][solution[trip1].locations[location1]].dist + adjMatrix[solution[trip1].locations[location1]][solution[trip1].locations[location1 + 1]].dist;

    // calcula distância percorrida no segmento anterior na trip 2
    double prevLength2 = adjMatrix[solution[trip2].locations[location2 - 1]][solution[trip2].locations[location2]].dist + adjMatrix[solution[trip2].locations[location2]][solution[trip2].locations[location2 + 1]].dist;

    // calcula distância percorrida no segmento posterior na trip 1
    double newLength1 = adjMatrix[solution[trip1].locations[location1 - 1]][solution[trip2].locations[location2]].dist + adjMatrix[solution[trip2].locations[location2]][solution[trip1].locations[location1 + 1]].dist;

    // calcula distância percorrida no segmento posterior na trip 2
    double newLength2 = adjMatrix[solution[trip2].locations[location2 - 1]][solution[trip1].locations[location1]].dist + adjMatrix[solution[trip1].locations[location1]][solution[trip2].locations[location2 + 1]].dist;

    // atualiza distância das trips, se o movimento for possível
    if (solution[trip1].tripLength + prevLength1 - newLength1 >= 0 && solution[trip2].tripLength + prevLength2 - newLength2 >= 0) {
        solution[trip1].tripLength += prevLength1 - newLength1;
        solution[trip2].tripLength += prevLength2 - newLength2;

        std::swap(solution[trip1].locations[location1], solution[trip2].locations[location2]);

        //std::cout << "Movimento realizado" << std::endl;
    }
}

void SA::swapBetweenTrips2(solution_t& solution){

    // escolhe duas trips diferentes com pelo menos duas localidades além dos hotéis
    int trip1 = rand() % solution.size();
    int trip2 = rand() % solution.size();

    while(solution[trip1].locations.size() <= 3)
        trip1 = rand() % solution.size();

    while(trip1 == trip2 || solution[trip2].locations.size() <= 3){
        trip2 = rand() % solution.size();
    }

    std::uniform_int_distribution<int> dist(1, solution[trip1].locations.size() - 3);
    int location1 = dist(rng);

    dist = std::uniform_int_distribution<int>(1, solution[trip2].locations.size() - 3);
    int location2 = dist(rng);

    // seleciona um local, seu vizinho e o sucessor do vizinho da primeira trip
    int swap11 = solution[trip1].locations[location1];
    int swap12 = solution[trip1].locations[location1 + 1];
    int sucessor1 = solution[trip1].locations[location1 + 2];
    int predecessor1 = solution[trip1].locations[location1 - 1];

    // seleciona um local, seu vizinho, o antecessor do local e o sucessor do vizinho da segunda trip
    int swap21 = solution[trip2].locations[location2];
    int swap22 = solution[trip2].locations[location2 + 1];
    int sucessor2 = solution[trip2].locations[location2 + 2];
    int predecessor2 = solution[trip2].locations[location2 - 1];

    //std::cout << "trocando (" << swap11 << "," << swap12 << ") com (" << swap21 << "," << swap22 << ")" << std::endl;

    // calcula distancia do segmento (antecessor1-dupla1-sucessor1) anterior na trip 1
    double pair1 = adjMatrix[swap11][swap12].dist;
    double prevLength1 = adjMatrix[predecessor1][swap11].dist + pair1 + adjMatrix[swap12][sucessor1].dist;

    // calcula distancia do segmento (antecessor2-dupla2-sucessor2) anterior na trip 2
    double pair2 = adjMatrix[swap21][swap22].dist;
    double prevLength2 = adjMatrix[predecessor2][swap21].dist + pair2 + adjMatrix[swap22][sucessor2].dist;

    // calcula distancia do segmento (antecessor1-dupla2-sucessor1) posterior na trip 1
    double newLength1 = adjMatrix[predecessor1][swap21].dist + pair2 + adjMatrix[swap22][sucessor1].dist;

    // calcula distancia do segmento (antecessor2-dupla1-sucessor2) posterior na trip 2
    double newLength2 = adjMatrix[predecessor2][swap11].dist + pair1 + adjMatrix[swap12][sucessor2].dist;

    // atualiza distancia das trips, se o movimento for possível
    if (solution[trip1].tripLength + prevLength1 - newLength1 >= 0 && solution[trip2].tripLength + prevLength2 - newLength2 >= 0) {
        solution[trip1].tripLength += prevLength1 - newLength1;
        solution[trip2].tripLength += prevLength2 - newLength2;

        std::swap(solution[trip1].locations[location1], solution[trip2].locations[location2]);
        std::swap(solution[trip1].locations[location1 + 1], solution[trip2].locations[location2 + 1]);

        //std::cout << "Movimento realizado" << std::endl;
    }
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

void SA::twoOpt(solution_t& solution){
    for (auto trip : solution) {
        int n = trip.locations.size();
        bool movement = false;

        // verifica se a distância da trip pode ser reduzida pelo movimento
        for (int i = 0; i <= n - 2; i++) {
			for (int j = i + 1; j <= n - 1; j++) {

                double trecho1 = adjMatrix[trip.locations[i]][trip.locations[(i + 1) % n]].dist;
                double trecho2 = adjMatrix[trip.locations[j]][trip.locations[(j + 1) % n]].dist;
                double trecho3 = adjMatrix[trip.locations[i]][trip.locations[j]].dist;
                double trecho4 = adjMatrix[trip.locations[(i + 1) % n]][trip.locations[(j + 1) % n]].dist;

                double delta = trecho1 + trecho2 - trecho3 - trecho4;

				if (delta < 0) {
					// realiza o movimento
                    //std::cout << "Movimento realizado" << std::endl;
                    std::reverse(std::begin(trip.locations) + i + 1, std::begin(trip.locations) + j + 1);
                    movement = true;

					// atualiza a distância da trip
                    trip.tripLength += delta;

                    break;
				}
			}

            if(movement)
                break;
		}
    }
}

Neighbor SA::generateNeighbor(solution_t currentSolution, tour_t& unvisitedLocations) {
    // faz movimentos

    Neighbor newSolution;
    newSolution.trips = currentSolution;

    //INSERT IF FEASIBLE
    //std::cout << std::endl << "INSERT IF FEASIBLE" << std::endl;
    newSolution.trips = insertIfFeasible(currentSolution, unvisitedLocations);
    newSolution.cost = objectiveFunction(newSolution.trips);

    if(newSolution.cost > this->currentSolution.cost)
        return newSolution;

    // SWAP IN TRIP
    // std::cout << std::endl << "SWAP IN TRIP" << std::endl;
    // swapInTrip(newSolution.trips);
    // newSolution.cost = objectiveFunction(newSolution.trips);
    
    // if(newSolution.cost > this->currentSolution.cost)
    //     return newSolution;

    // SWAP(1,1)
    //std::cout << std::endl << "SWAP(1,1)" << std::endl;
    swapBetweenTrips(newSolution.trips);
    newSolution.cost = objectiveFunction(newSolution.trips);

    if(newSolution.cost > this->currentSolution.cost)
        return newSolution;

    // SWAP(2,2)
    //std::cout << std::endl << "SWAP(2,2)" << std::endl;
    swapBetweenTrips2(newSolution.trips);
    newSolution.cost = objectiveFunction(newSolution.trips);

    if(newSolution.cost > this->currentSolution.cost)
        return newSolution;

    // 2-OPT 
    //std::cout << "2-OPT" << std::endl;
    twoOpt(newSolution.trips);
    newSolution.cost = objectiveFunction(newSolution.trips);

    return newSolution;
}

void SA::printSolution() {
    double tourScore = 0.0;
    std::cout << "Best solution: " << bestSolution.cost << std::endl << std::endl;
    std::cout << "TRIPS: " << std::endl;
    int t = 1;
    for (auto trip : bestSolution.trips) {
        double tripScore = 0.0;

        for (int i = 0; i < trip.locations.size() - 1; i++) {
            int from = trip.locations[i];
            int to = trip.locations[i + 1];
            tripScore += adjMatrix[from][to].score;
        }
        
        tourScore += tripScore;
        std::cout << "Trip " << t++;
        std::cout << " | Length: " << trip.tripLength << " | Score: " << tripScore << " | ";

        for (auto location : trip.locations) {
            std::cout << location << " ";
        }
        
        std::cout << std::endl;
    }

    std::cout << "-----------------" << std::endl;
    std::cout << "Tour Score: " << tourScore << std::endl;
}

void SA::printSolution(Neighbor solution) {
    double tourScore = 0.0;
    
    std::cout << "New solution: " << solution.cost << std::endl << std::endl;
    std::cout << "TRIPS: " << std::endl;
    int t = 1;
    for (auto trip : solution.trips) {
        double tripScore = 0.0;

        for (int i = 0; i < trip.locations.size() - 1; i++) {
            int from = trip.locations[i];
            int to = trip.locations[i + 1];
            tripScore += adjMatrix[from][to].score;
        }
        
        tourScore += tripScore;
        std::cout << "Trip " << t++;
        std::cout << " | Length: " << trip.tripLength << " | Score: " << tripScore << " | ";

        for (auto location : trip.locations) {
            std::cout << location << " ";
        }
        
        std::cout << std::endl;
    }

    std::cout << "-----------------" << std::endl;
    std::cout << "Tour Score: " << tourScore << std::endl;
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
    // insertIfFits(bestSolution.trips, unvisitedLocations);
    printSolution();
}
