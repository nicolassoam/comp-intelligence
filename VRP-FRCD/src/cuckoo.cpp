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

list_t constructCandidateList(Instance* inst, Vehicle v, std::vector<std::pair<double, availability>>& availableSuppliers, list_t& candidateList, TYPE type){

    double heuristic = 0;

    // ranking candidate node k to be inserted between nodes i and j
    // delta dist = dist(i,k) + dist(k,j) - dist(i,j)
    double deltaDist = 0;
    double deltaCost = 0;

    // at maximum, should use n vehicles, n as number of suppliers

    std::vector<int>routes = v.getRoutes();

    candidateList.clear();

    for (int i = 0; i < availableSuppliers.size(); i++){

        if (availableSuppliers[i].second != AVAILABLE) continue;

        for (int j = 0; j < routes.size()-1; j++){
            switch(type){
                case RETAILER:
                    deltaDist = inst->retailerCrossDockDist[routes[j]][i+1] + inst->retailerCrossDockDist[i+1][routes[j+1]] - inst->retailerCrossDockDist[routes[j]][routes[j+1]];
                    break;
                case OUTLETS:
                    deltaDist = inst->outletCrossDockDist[routes[j]][i+1] + inst->outletCrossDockDist[i+1][routes[j+1]] - inst->outletCrossDockDist[routes[j]][routes[j+1]];
                    break;
                case SUPPLIER:
                    deltaDist = inst->supplierCrossDockDist[routes[j]][i+1] + inst->supplierCrossDockDist[i+1][routes[j+1]] - inst->supplierCrossDockDist[routes[j]][routes[j+1]];
                    break;
            }
            deltaCost = inst->c * deltaDist + inst->COST;
            heuristic = deltaCost/deltaDist;

            if (heuristic < 0) continue;
            
            candidateList.push_back(std::make_tuple(heuristic, i+1, routes[j], routes[j+1]));
        }
    }
    
    return candidateList;
}

void MCS::supplierInit(cuckoo& cuckoo){
   
    std::vector<std::pair<double, availability>> demandPerProduct = this->inst->demandPerProduct;
    list_t candidateList;

    int k = cuckoo.usedVehicles;
    int attended = 0;
    int kSup = 0, iSup = 0, jSup = 0;

    while (cuckoo.usedVehicles <= this->nVehicles){
        
        if (demandPerProduct.size() == attended) break;

        cuckoo.vehicles[k].setType(SUPPLIER);
        cuckoo.vehicleTypes[SUPPLIER]++;

        do {
            constructCandidateList(this->inst, cuckoo.vehicles[k], demandPerProduct, candidateList, SUPPLIER);
            std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

            if (candidateList.empty()) break;

            kSup = std::get<1>(candidateList.front());
            iSup = std::get<2>(candidateList.front());
            jSup = std::get<3>(candidateList.front());

            double capacity = cuckoo.vehicles[k].getCapacity() - demandPerProduct[kSup-1].first;

            if (capacity < 0) {

                demandPerProduct[kSup-1].second = UNAVAILABLE;
                if (attended == demandPerProduct.size()) break;

                continue;
            } 
            
            cuckoo.vehicles[k].insertBetween(iSup, jSup, kSup);
            demandPerProduct[kSup-1].second = VISITED;
            cuckoo.vehicles[k].setCapacity(capacity);

            attended++;

            if (demandPerProduct.size()-1 == attended) break;

        } while (!candidateList.empty());
        
        for (auto& demand : demandPerProduct) {
            if (demand.second == UNAVAILABLE)
                demand.second = AVAILABLE;
        }

        cuckoo.usedVehicles++;
        k++;
    }
}

void MCS::outletInit(cuckoo& cuckoo){
    std::vector<std::pair<double,availability>>demandPerOutlet = this->inst->outletDemand;
    list_t candidateList;

    int k = cuckoo.usedVehicles;
    int attended = 0;
    int kSup = 0, iSup = 0, jSup = 0;

    while(cuckoo.usedVehicles <= this->nVehicles){
        
        if(demandPerOutlet.size() == attended) break;
        cuckoo.vehicles[k].setType(OUTLETS);
        cuckoo.vehicleTypes[OUTLETS]++;

        do{
            constructCandidateList(this->inst, cuckoo.vehicles[k], demandPerOutlet, candidateList, OUTLETS);
            std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

            if(candidateList.empty()) break;

            kSup = std::get<1>(candidateList.front());
            iSup = std::get<2>(candidateList.front());
            jSup = std::get<3>(candidateList.front());

            double capacity = cuckoo.vehicles[k].getCapacity() - demandPerOutlet[kSup-1].first;

            if(capacity < 0){
                demandPerOutlet[kSup-1].second = UNAVAILABLE;
                if(attended == demandPerOutlet.size()) break;
                continue;
            }

            cuckoo.vehicles[k].insertBetween(iSup, jSup, kSup);
            demandPerOutlet[kSup-1].second = VISITED;
            cuckoo.vehicles[k].setCapacity(capacity);

            attended++;

            if(demandPerOutlet.size()-1 == attended) break;

        }while(!candidateList.empty());

        for(auto& demand : demandPerOutlet){
            if(demand.second == UNAVAILABLE)
                demand.second = AVAILABLE;
        }

        cuckoo.usedVehicles++;
        k++;
    }
}

void MCS::retailerInit(cuckoo& cuckoo){
    std::vector<std::pair<double, availability>> demandPerRetailer = this->inst->demandPerRetailer;
    list_t candidateList;

    int k = cuckoo.usedVehicles;
    int attended = 0;
    int kSup = 0, iSup = 0, jSup = 0;

    while (cuckoo.usedVehicles <= this->nVehicles){
        
        if (demandPerRetailer.size() == attended) break;

        cuckoo.vehicles[k].setType(RETAILER);
        cuckoo.vehicleTypes[RETAILER]++;

        do {
            constructCandidateList(this->inst, cuckoo.vehicles[k], demandPerRetailer, candidateList, RETAILER);
            std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

            if (candidateList.empty()) break;

            kSup = std::get<1>(candidateList.front());
            iSup = std::get<2>(candidateList.front());
            jSup = std::get<3>(candidateList.front());

            double capacity = cuckoo.vehicles[k].getCapacity() - demandPerRetailer[kSup-1].first;

            if (capacity < 0) {

                demandPerRetailer[kSup-1].second = UNAVAILABLE;
                if (attended == demandPerRetailer.size()) break;

                continue;
            } 
            
            cuckoo.vehicles[k].insertBetween(iSup, jSup, kSup);
            demandPerRetailer[kSup-1].second = VISITED;
            cuckoo.vehicles[k].setCapacity(capacity);

            attended++;

            if (demandPerRetailer.size()-1 == attended) break;

        } while (!candidateList.empty());
        
        for (auto& demand : demandPerRetailer) {
            if (demand.second == UNAVAILABLE)
                demand.second = AVAILABLE;
        }

        cuckoo.usedVehicles++;
        k++;
    }
}

void MCS::initPopulation(){

    // generate initial nNests solutions with nVehicles vehicles
    for (int i = 0; i < nNests; i++){
        cuckoo newCuckoo;
        newCuckoo.vehicles = Vehicle::instantiateVehicles(nVehicles, this->inst->COST, this->inst->capacity);

        // init retailers routes
        retailerInit(newCuckoo);

        // init outlets routes
        outletInit(newCuckoo);

        // init suppliers routes
        supplierInit(newCuckoo);

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
    // for each type of vehicle
    for (int i = 0; i < 3; i++) {

        random1 = -1;
        random2 = -1;

        // choose two random routes of type i
        if (newCuckoo.vehicleTypes[i] >= 2) {
            std::uniform_int_distribution<> dis2(0, newCuckoo.vehicleTypes[i]-1);
            random1 = dis2(rd);

            do {
                random2 = dis2(rd);
            } while (random1 == random2);
            
            switch (i) {
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
    int nSteps =0;
    double num = 1;

    std::cout << "Initializing Search" << std::endl;
    initPopulation();

    nSteps=6;
    num /= nSteps+1;

    //initialize vector of iterator, ranging from 0 to 1
    std::vector<double> iteratorVector;
    for (int i = 0; i < nSteps; i++){
        iteratorVector.push_back(num);
        num += num;
    }


    std::random_device rd;
    
    //initial fitness
    std::cout << "Calculating initial fitness" << std::endl;
    for (int i = 0; i < nNests; i++)
    {
        nests[i].fitness = fitness(nests[i].vehicles, nests[i].usedVehicles);
    }

    int generation = 1;
    std::cout << "Starting Search" << std::endl;

    for(int i = 0; i < this->nIterations; i++){
        //choose a random nest
        generation++;
        std::sort(nests.begin(), nests.end(), [](cuckoo &a, cuckoo &b) {return a.fitness < b.fitness;});
        std::cout << "Iteration: " << i + 1 << " Best Fitness: " << nests[0].fitness << std::endl;

        int s = (int) (pa * nNests);
        
        for (int j = nNests - s; j < nNests; j++)
        {
            levyStepSize = maxLevyStepSize/(std::sqrt(generation));
           
            nests[j] = applyMovement(nests[j], iteratorVector);
            
            nests[j].fitness = fitness(nests[j].vehicles, nests[j].usedVehicles);
        }

        std::uniform_int_distribution<> dis(0, nNests - s);

        for(int j = 0; j < nNests - s; j++){
            
            int randomNest = dis(rd);
            if (randomNest == j) {
                levyStepSize = maxLevyStepSize/(std::pow(generation, 2));

                cuckoo newCuckoo = applyMovement(nests[j], iteratorVector);

                int randomNest2 = dis(rd);
                double newFitness = fitness(newCuckoo.vehicles, newCuckoo.usedVehicles);
                if(newFitness < nests[randomNest2].fitness){
                    nests[randomNest2].vehicles = newCuckoo.vehicles;
                    nests[randomNest2].usedVehicles = newCuckoo.usedVehicles;
                    nests[randomNest2].fitness = newFitness;
                }
            } else {
                cuckoo cuckooK = nests[j];

                //random movement
                std::uniform_int_distribution<> dis2(0, nSteps-1);

                int randomMovement = dis2(rd);
                int randomVehicle1 = -1, randomVehicle2 = -1;

                selectRandomRoutes(randomVehicle1, randomVehicle2, cuckooK);


                switch(randomMovement){
                    case 0:
                        Neighborhood::shift1_0(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
                        break;
                    case 1:
                        Neighborhood::reinsertion(cuckooK.vehicles[randomVehicle1], this->inst);
                        break;
                    case 2:
                        Neighborhood::twoOpt(cuckooK.vehicles[randomVehicle1], this->inst);
                        break;
                    case 3:
                        Neighborhood::exchange(cuckooK.vehicles[randomVehicle1]);
                        break;
                    case 4:
                        Neighborhood::swap2_1(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
                        break;
                    case 5:
                        Neighborhood::cross(cuckooK.vehicles[randomVehicle1], cuckooK.vehicles[randomVehicle2], this->inst);
                        break;

                }
                
                cuckooK.fitness = fitness(cuckooK.vehicles, cuckooK.usedVehicles);
                int randomNest2 = dis(rd);

                if(cuckooK.fitness < nests[randomNest2].fitness){
                    nests[randomNest2].vehicles = cuckooK.vehicles;
                    nests[randomNest2].usedVehicles = cuckooK.usedVehicles;
                    nests[randomNest2].fitness = cuckooK.fitness;
                }
            }
        }
    }

    std::cout << "Search Finished" << std::endl;
    
    printSolution();

    return;
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