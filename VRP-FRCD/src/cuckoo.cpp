#include "../include/cuckoo.hpp"
#include "../include/neighborhood.hpp"


MCS::MCS( int nNests, int nVehicles, int nIterations, Instance* inst){
    this->nNests = nNests;
    this->nVehicles = nVehicles;
    this->nIterations = nIterations;
    this->levyStepSize = 1;
    this->inst = inst;
}

double MCS::levyFlight(){
    
    // levy distribution
    double lambda = 1.5;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::gamma_distribution<double> gamma(1,2);
    double num = gamma(rd)*(1+lambda) * std::sin((std::numbers::pi * lambda) / 2);
    double den = gamma(rd) * ((1 + lambda) / 2) * lambda * std::pow(2, ((lambda - 1) / 2));
    double sigma = std::pow((num / den), (1 / lambda));

    std::normal_distribution<double> u_n(0, sigma);
    std::normal_distribution<double> v_n(0, 1.0);

    double u = u_n(gen);
    double v = v_n(gen);
    double step = std::abs(u) / std::pow(std::abs(v), (1 / lambda));
    
    return levyStepSize * step;
}

// custo de transporte por unidade de distancia, multiplicado pela distancia
// c * dist + H * V_utilizados
double MCS::fitness(v vehicles, int usedVehicles){
    double fitness = 0;
    for(int i = 0; i< usedVehicles; i++){
        std::vector<int>route = vehicles[i].getRoutes();
        for(int j = 0; j < route.size()-1;j++){
            switch(vehicles[i].getType()){
                case RETAILER:
                    fitness += this->inst->c * inst->retailerCrossDockDist[route[j]][route[j+1]];
                    break;
                case SUPPLIER:
                    fitness += this->inst->c * inst->supplierCrossDockDist[route[j]][route[j+1]];
                    break;
                case OUTLETS:
                    fitness += this->inst->c * inst->outletCrossDockDist[route[j]][route[j+1]];
                    break;
            }
        }
    }
    return fitness + this->inst->COST * usedVehicles;

}

list_t initCandidateList(Instance* inst, list_t& candidateList) {
    double heuristic = 0;

    // ranking candidate node k to be inserted between nodes i and j
    // delta dist = dist(i,k) + dist(k,j) - dist(i,j)

    double deltaDist = 0;
    double deltaTime = 0;

    for (int v = 0; v < inst->nVehicles; v++) {

        // FOR RETAILER
        for (int r = 0; r < inst->demandPerRetailer.size(); r++) {
            if (v == 0) {
                deltaDist = inst->retailerCrossDockDist[0][r+1] + inst->retailerCrossDockDist[r+1][1] - inst->retailerCrossDockDist[0][1];
                deltaTime = inst->retailerCrossDockTime[0][r+1] + inst->retailerCrossDockTime[r+1][1] - inst->retailerCrossDockTime[0][1];

                //deltaTime = deltaTime + (deltaTime*RETAILER);

                heuristic = deltaDist*deltaTime;

                candidateList.push_back(std::make_tuple(heuristic, r+1, 0, 1, RETAILER, v));
                continue;
            };

            deltaDist = inst->retailerCrossDockDist[0][r+1] + inst->retailerCrossDockDist[r+1][0] - inst->retailerCrossDockDist[0][0];
            deltaTime = inst->retailerCrossDockTime[0][r+1] + inst->retailerCrossDockTime[r+1][0] - inst->retailerCrossDockTime[0][0];

            //deltaTime = deltaTime + (deltaTime*RETAILER);

            heuristic = deltaDist*deltaTime;

            candidateList.push_back(std::make_tuple(heuristic, r+1, 0, 0, RETAILER, v));
        }

        // FOR OUTLETS
        for (int o = 0; o < inst->outletDemand.size(); o++) {
            if (v == 1) {
                deltaDist = inst->outletCrossDockDist[0][o+1] + inst->outletCrossDockDist[o+1][1] - inst->outletCrossDockDist[0][1];
                deltaTime = inst->outletCrossDockTime[0][o+1] + inst->outletCrossDockTime[o+1][1] - inst->outletCrossDockTime[0][1];

                //deltaTime = deltaTime + (deltaTime*OUTLETS);

                heuristic = deltaDist*deltaTime;

                candidateList.push_back(std::make_tuple(heuristic, o+1, 0, 1, OUTLETS, v));
                continue;
            };

            deltaDist = inst->outletCrossDockDist[0][o+1] + inst->outletCrossDockDist[o+1][0] - inst->outletCrossDockDist[0][0];
            deltaTime = inst->outletCrossDockTime[0][o+1] + inst->outletCrossDockTime[o+1][0] - inst->outletCrossDockTime[0][0];

            //deltaTime = deltaTime + (deltaTime*OUTLETS);

            heuristic = deltaDist*deltaTime;

            candidateList.push_back(std::make_tuple(heuristic, o+1, 0, 0, OUTLETS, v));
        }

        // FOR SUPPLIERS
        for (int s = 0; s < inst->demandPerProduct.size(); s++) {
            if (v == 2) {
                deltaDist = inst->supplierCrossDockDist[0][s+1] + inst->supplierCrossDockDist[s+1][1] - inst->supplierCrossDockDist[0][1];
                deltaTime = inst->supplierCrossDockTime[0][s+1] + inst->supplierCrossDockTime[s+1][1] - inst->supplierCrossDockTime[0][1];

                //deltaTime = deltaTime + (deltaTime*SUPPLIER);

                heuristic = deltaDist*deltaTime;

                candidateList.push_back(std::make_tuple(heuristic, s+1, 0, 1, SUPPLIER, v));
                continue;
            }

            deltaDist = inst->supplierCrossDockDist[0][s+1] + inst->supplierCrossDockDist[s+1][0] - inst->supplierCrossDockDist[0][0];
            deltaTime = inst->supplierCrossDockTime[0][s+1] + inst->supplierCrossDockTime[s+1][0] - inst->supplierCrossDockTime[0][0];

            //deltaTime = deltaTime + (deltaTime*SUPPLIER);

            heuristic = deltaDist*deltaTime;

            candidateList.push_back(std::make_tuple(heuristic, s+1, 0, 0, SUPPLIER, v));
        }
    }

    return candidateList;
}

void updateCandidateList(Instance* inst, list_t& candidateList, matrix& timeMatrix, matrix& distanceMatrix, cuckoo& cuckoo, candidate inserted, Neighborhood::av& demand) {
    
    double heuristic = 0;
    double deltaDist = 0;
    double deltaTime = 0;

    int k = std::get<1>(inserted);
    int i = std::get<2>(inserted);
    int j = std::get<3>(inserted);
    TYPE type = std::get<4>(inserted);
    int vehicleId = std::get<5>(inserted);

    // goes through candidate list
    for (auto it = candidateList.begin(); it != candidateList.end();) {
        
        // if candidate is not available anymore, remove it from list
        if (std::get<4>(*it) == type && std::get<1>(*it) == k) {
            //std::cout << "(NA) removing candidate " << std::get<1>(*it) << " | " << std::get<0>(*it) << std::endl;
            it = candidateList.erase(it);
            continue;
        } 

        // if candidate of another type was available for new route, remove it from list
        if (std::get<4>(*it) != type && std::get<5>(*it) == vehicleId) {
            //std::cout << "(TYPE ROUTE) removing candidate " << std::get<1>(*it) << " | " << std::get<0>(*it) << std::endl;
            it = candidateList.erase(it);
            continue;
        }

        // if candidate was to be inserted between i and j, remove it from list
        if (std::get<2>(*it) == i && std::get<3>(*it) == j && std::get<5>(*it) == vehicleId) {
            //std::cout << "(ij) removing candidate " << std::get<1>(*it) << " | " << std::get<5>(*it)<< " | " << std::get<0>(*it) << std::endl;
            it = candidateList.erase(it);
            continue;
        }

        it++;
    }


    for (int id = 1; id <= demand.size(); id++) {
        
        if (demand[id-1].second == VISITED) continue;

        // between i and k
        deltaDist = distanceMatrix[i][id] + distanceMatrix[id][k] - distanceMatrix[i][k];
        deltaTime = timeMatrix[i][id] + timeMatrix[id][k] - timeMatrix[i][k];
        
        if (deltaTime + cuckoo.currentTime > inst->tMax) continue;
        
        deltaTime = deltaTime + (deltaTime*type);
        heuristic = deltaDist*deltaTime;
        
        if (heuristic <= 0) continue;
        
        candidateList.push_back(std::make_tuple(heuristic, id, i, k, type, vehicleId));

        // between k and j    
        deltaDist = distanceMatrix[k][id] + distanceMatrix[id][j] - distanceMatrix[k][j];
        deltaTime = timeMatrix[k][id] + timeMatrix[id][j] - timeMatrix[k][j];

        if (deltaTime + cuckoo.currentTime > inst->tMax) continue;
        
        deltaTime = deltaTime + (deltaTime*type);
        heuristic = deltaDist*deltaTime;
        
        if (heuristic <= 0) continue;
        
        candidateList.push_back(std::make_tuple(heuristic, id, k, j, type, vehicleId));
    }
    
}

bool insertInRoute(candidate& candidate, std::vector<std::pair<double, availability>>& demand, cuckoo& cuckoo, matrix& timeMatrix, Instance* inst) {
    int kSup = std::get<1>(candidate);
    int iSup = std::get<2>(candidate);
    int jSup = std::get<3>(candidate);
    TYPE type = std::get<4>(candidate);
    int vehicle = std::get<5>(candidate);

    if (demand[kSup-1].second == VISITED) return false;

    double capacity = cuckoo.vehicles[vehicle].getCapacity() - demand[kSup-1].first;
    double time = cuckoo.currentTime + timeMatrix[iSup][kSup] + timeMatrix[kSup][jSup] - timeMatrix[iSup][jSup];

    if (capacity < 0 || time > inst->tMax) {
        std::cout << "time or capacity limit exceeded" << std::endl;
        //demand[kSup-1].second = UNAVAILABLE;
        return false;
    }

    cuckoo.vehicles[vehicle].insertBetween(iSup, jSup, kSup);
    cuckoo.vehicles[vehicle].setCapacity(capacity);
    cuckoo.currentTime = time;
    demand[kSup-1].second = VISITED;
    return true;
}

void MCS::initRoutes(cuckoo& cuckoo){
    std::vector<std::pair<double, availability>> demandPerRetailer = this->inst->demandPerRetailer;
    std::vector<std::pair<double, availability>> demandPerProduct = this->inst->demandPerProduct;
    std::vector<std::pair<double, availability>> demandPerOutlet = this->inst->outletDemand;
    
    list_t candidateList;

    int k = cuckoo.usedVehicles;
    int attendedRet, attendedOut, attendedSup;
    attendedRet = attendedOut = attendedSup = 1;
    int kSup = 0, iSup = 0, jSup = 0, vehicle = 0;
    double capacity = 0, time = 0;
    TYPE type;

    // insert one node of each type on vehicle
    candidate cand = std::make_tuple(0, 1, 0, 0, RETAILER, 0);
    insertInRoute(cand, demandPerRetailer, cuckoo, inst->retailerCrossDockTime, inst);
    cuckoo.vehicles[0].setType(RETAILER);

    cand = std::make_tuple(0, 1, 0, 0, OUTLETS, 1);
    insertInRoute(cand, demandPerOutlet, cuckoo, inst->outletCrossDockTime, inst);
    demandPerOutlet[0].second = VISITED;
    cuckoo.vehicles[1].setType(OUTLETS);

    cand = std::make_tuple(0, 1, 0, 0, SUPPLIER, 2);
    insertInRoute(cand, demandPerProduct, cuckoo, inst->supplierCrossDockTime, inst);
    demandPerProduct[0].second = VISITED;
    cuckoo.vehicles[2].setType(SUPPLIER);

    initCandidateList(this->inst, candidateList);
    std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

    // print candidate list
    for (auto& candidate : candidateList) {
        std::cout << std::get<0>(candidate) << " | " << std::get<1>(candidate) << " | " << std::get<2>(candidate) << " | " << std::get<3>(candidate) << " | " << std::get<4>(candidate) << " | " << std::get<5>(candidate) << std::endl;
    }
    std::cout << std::endl;

    while (!candidateList.empty()) {

        type = std::get<4>(candidateList.front());
        vehicle = std::get<5>(candidateList.front());
        bool insertion = false;

        switch (type) {
            case RETAILER:
                insertion = insertInRoute(candidateList.front(), demandPerRetailer, cuckoo, inst->retailerCrossDockTime, inst);
                if (insertion) {
                    std::cout << "inserted retailer " << std::get<1>(candidateList.front()) << " on vehicle " << std::get<5>(candidateList.front()) << std::endl;
                    attendedRet++;

                    if (cuckoo.vehicles[vehicle].getType() != RETAILER) {
                        cuckoo.vehicles[vehicle].setType(RETAILER);
                        cuckoo.vehicleTypes[RETAILER]++;
                    }

                    updateCandidateList(this->inst, candidateList, inst->retailerCrossDockTime, inst->retailerCrossDockDist, cuckoo, candidateList.front(), demandPerRetailer);
                    
                    if (demandPerRetailer.size()-1 == attendedRet) break;

                } else {
                    // remove candidate
                    candidateList.erase(candidateList.begin());
                }
                break;

            case OUTLETS:
                insertion = insertInRoute(candidateList.front(), demandPerOutlet, cuckoo, inst->outletCrossDockTime, inst);
                if (insertion) {
                    std::cout << "inserted outlet " << std::get<1>(candidateList.front()) << " on vehicle " << std::get<5>(candidateList.front()) << std::endl;
                    attendedOut++;
                    
                    if (cuckoo.vehicles[vehicle].getType() != OUTLETS) {
                        cuckoo.vehicles[vehicle].setType(OUTLETS);
                        cuckoo.vehicleTypes[OUTLETS]++;
                    }

                    updateCandidateList(this->inst, candidateList, inst->outletCrossDockTime, inst->outletCrossDockDist, cuckoo, candidateList.front(), demandPerOutlet);

                    if (demandPerOutlet.size()-1 == attendedOut) break;

                } else {
                    // remove candidate
                    candidateList.erase(candidateList.begin());
                }
                break;

            case SUPPLIER:
                insertion = insertInRoute(candidateList.front(), demandPerProduct, cuckoo, inst->supplierCrossDockTime, inst);
                if (insertion) {
                    std::cout << "inserted supplier " << std::get<1>(candidateList.front()) << " on vehicle " << std::get<5>(candidateList.front()) << std::endl;
                    attendedSup++;

                    if (cuckoo.vehicles[vehicle].getType() != SUPPLIER) {
                        cuckoo.vehicles[vehicle].setType(SUPPLIER);
                        cuckoo.vehicleTypes[SUPPLIER]++;
                    }
                    
                    updateCandidateList(this->inst, candidateList, inst->supplierCrossDockTime, inst->supplierCrossDockDist, cuckoo, candidateList.front(), demandPerProduct);
                    
                    if (demandPerProduct.size()-1 == attendedSup) break;

                } else {
                    // remove candidate
                    candidateList.erase(candidateList.begin());
                }
                break;
        }

        std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

        //print candidate list
        for (auto& candidate : candidateList) {
            std::cout << std::get<0>(candidate) << " | " << std::get<1>(candidate) << " | " << std::get<2>(candidate) << " | " << std::get<3>(candidate) << " | " << std::get<4>(candidate) << " | " << std::get<5>(candidate) << std::endl;
        }
        std::cout << std::endl;

        if (attendedOut == demandPerOutlet.size() && attendedRet == demandPerRetailer.size() && attendedSup == demandPerProduct.size())
            break;
    }
}

void MCS::initPopulation(){

    // generate initial nNests solutions with nVehicles vehicles
    for (int i = 0; i < nNests; i++){
        cuckoo newCuckoo;
        newCuckoo.vehicles = Vehicle::instantiateVehicles(nVehicles, this->inst->COST, this->inst->capacity);

        initRoutes(newCuckoo);

        for(Vehicle & v: newCuckoo.vehicles){
            v.shuffle();
        }

        nests.push_back(newCuckoo);
    } 

    for(auto nest : nests){
        for(auto vehicle : nest.vehicles){
            std::cout << vehicle << std::endl;
        }
    }

    //exit(1);
}

void selectRandomRoutes(int &random1,int &random2,cuckoo newCuckoo){
    std::random_device rd;
    // defines type of vehicle
    std::uniform_int_distribution<> dis(0, 2);
    int type = dis(rd);

        random1 = -1;
        random2 = -1;

        // choose two random routes of type i
        if (newCuckoo.vehicleTypes[type] >= 2) {
            std::uniform_int_distribution<> dis2(0, newCuckoo.vehicleTypes[type]-1);
            random1 = dis2(rd);

            do {
                random2 = dis2(rd);
            } while (random1 == random2);
            
            switch (type) {
                case RETAILER:
                    // std::cout << "RETAILER" << std::endl;
                    break;
                case OUTLETS:
                    // std::cout << "OUTLETS" << std::endl;
                    random1 = random1 + newCuckoo.vehicleTypes[RETAILER];
                    random2 = random2 + newCuckoo.vehicleTypes[RETAILER];
                    break;
                case SUPPLIER:
                    // std::cout << "SUPPLIER" << std::endl;
                    random1 = random1 + newCuckoo.vehicleTypes[RETAILER] + newCuckoo.vehicleTypes[OUTLETS];
                    random2 = random2 + newCuckoo.vehicleTypes[RETAILER] + newCuckoo.vehicleTypes[OUTLETS];
                    break;
            }
        }
}

cuckoo MCS::applyMovement(cuckoo c, std::vector<double>iteratorVector){

    cuckoo newCuckoo = c;
    
    double flight = levyFlight();
    int random1 = -1, random2 = -1;

    selectRandomRoutes(random1, random2, newCuckoo);

    // std::cout << "random routes: " << random1 << " " << random2 << std::endl;
    if(flight>=0 && flight <= iteratorVector[0]){
        // std::cout << "shift" << std::endl;
        Neighborhood::shift1_0(newCuckoo.vehicles[random1], newCuckoo.vehicles[random2], this->inst);
    } else if(flight >=iteratorVector[0] && flight <= iteratorVector[1]){
        // std::cout << "reinsertion"<<std::endl;
        Neighborhood::reinsertion(newCuckoo.vehicles[random1], this->inst);
    } else if(flight >=iteratorVector[1] && flight <= iteratorVector[2]){
        // std::cout << "twopt" << std::endl;
        Neighborhood::twoOpt(newCuckoo.vehicles[random1], this->inst);
    } else if(flight >=iteratorVector[2] && flight <= iteratorVector[3]){
        // std::cout << "exchange" << std::endl;
        Neighborhood::exchange(newCuckoo.vehicles[random1]);
    } else if(flight >= iteratorVector[3] && flight <= iteratorVector[4]){
        // std::cout << "swap" << std::endl;
        Neighborhood::swap2_1(newCuckoo.vehicles[random1], newCuckoo.vehicles[random2], this->inst);
    } else if(flight >= iteratorVector[4] && flight <= iteratorVector[5]){
        // std::cout << "cross" << std::endl;
        Neighborhood::cross(newCuckoo.vehicles[random1], newCuckoo.vehicles[random2], this->inst);
    }
        

    return newCuckoo;
}

void MCS::search(){

    // n movements 
    int nSteps = 0;
    double num = 1;

    std::cout << "Initializing Search" << std::endl;
    initPopulation();

    // nSteps=6;
    // num /= nSteps+1;

    // //initialize vector of iterator, ranging from 0 to 1
    // std::vector<double> iteratorVector;
    // for (int i = 0; i < nSteps; i++){
    //     iteratorVector.push_back(num);
    //     num += num;
    // }


    // std::random_device rd;
    
    // //initial fitness
    // std::cout << "Calculating initial fitness" << std::endl;
    // for (int i = 0; i < nNests; i++)
    // {
    //     nests[i].fitness = fitness(nests[i].vehicles, nests[i].usedVehicles);
    // }

    // int generation = 1;
    // std::cout << "Starting Search" << std::endl;

    // for(int i = 0; i < this->nIterations; i++){
    //     //choose a random nest
    //     generation++;
    //     std::sort(nests.begin(), nests.end(), [](cuckoo &a, cuckoo &b) {return a.fitness < b.fitness;});
    //     std::cout << "Iteration: " << i + 1 << " Best Fitness: " << nests[0].fitness << std::endl;

    //     int s = (int) (pa * nNests);
    //     levyStepSize = maxLevyStepSize/(std::sqrt(generation));

    //     for (int j = nNests - s; j < nNests; j++)
    //     {
    //         nests[j] = applyMovement(nests[j], iteratorVector);   
    //         nests[j].fitness = fitness(nests[j].vehicles, nests[j].usedVehicles);
    //     }

    //     std::uniform_int_distribution<> dis(0, nNests - s);
    //     levyStepSize = maxLevyStepSize/(std::pow(generation, 2));
        
    //     for(int j = 0; j < nNests - s; j++){
            
    //         int randomNest = dis(rd);
    //         if (randomNest == j) {
    //             cuckoo newCuckoo = applyMovement(nests[j], iteratorVector);

    //             int randomNest2 = dis(rd);
    //             double newFitness = fitness(newCuckoo.vehicles, newCuckoo.usedVehicles);
    //             if(newFitness < nests[randomNest2].fitness){
    //                 nests[randomNest2].vehicles = newCuckoo.vehicles;
    //                 nests[randomNest2].usedVehicles = newCuckoo.usedVehicles;
    //                 nests[randomNest2].fitness = newFitness;
    //             }
    //         } else {
    //             cuckoo cuckooK = nests[j];

    //             //random movement
    //             std::uniform_int_distribution<> dis2(0, nSteps-1);

    //             int randomMovement = dis2(rd);
    //             int randomVehicle1 = -1, randomVehicle2 = -1;

    //             selectRandomRoutes(randomVehicle1, randomVehicle2, cuckooK);


    //             switch(randomMovement){
    //                 case 0:
    //                     Neighborhood::shift1_0(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
    //                     break;
    //                 case 1:
    //                     Neighborhood::reinsertion(cuckooK.vehicles[randomVehicle1], this->inst);
    //                     break;
    //                 case 2:
    //                     Neighborhood::twoOpt(cuckooK.vehicles[randomVehicle1], this->inst);
    //                     break;
    //                 case 3:
    //                     Neighborhood::exchange(cuckooK.vehicles[randomVehicle1]);
    //                     break;
    //                 case 4:
    //                     Neighborhood::swap2_1(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
    //                     break;
    //                 case 5:
    //                     Neighborhood::cross(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
    //                     break;

    //             }
                
    //             cuckooK.fitness = fitness(cuckooK.vehicles, cuckooK.usedVehicles);
    //             int randomNest2 = dis(rd);

    //             if(cuckooK.fitness < nests[randomNest2].fitness){
    //                 nests[randomNest2].vehicles = cuckooK.vehicles;
    //                 nests[randomNest2].usedVehicles = cuckooK.usedVehicles;
    //                 nests[randomNest2].fitness = cuckooK.fitness;
    //             }
    //         }
    //     }
    // }

    // std::cout << "Search Finished" << std::endl;
    
    //printSolution();

    return;
}

cuckoo MCS::getBestCuckoo(){
    return nests[0];
}

void MCS::printSolution(){
    std::cout << "Best Solution: " << std::endl;
    for(int i = 0; i < nVehicles; i++){
        std::cout << nests[0].vehicles[i] << " ";
    }
    return;
}

MCS::~MCS(){
    nests.clear();
    return;
}