#include "../include/cuckoo.hpp"


MCS::MCS( int nNests, int nVehicles, int nIterations, solution_t lowerBound, solution_t upperBound, Instance* inst){
    this->nNests = nNests;
    this->nVehicles = nVehicles;
    this->nIterations = nIterations;
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->levyStepSize = maxLevyStepSize;
    this->inst = inst;
}

cuckoo MCS::newCuckoo (int nest){
    cuckoo newCuckoo;
    newCuckoo.solution = nests[nest].solution;
    //levy distribution
    double lambda = 1.5;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double num = std::tgamma(1 + lambda) * std::sin((std::numbers::pi * lambda) / 2);
    double den = std::tgamma((1 + lambda) / 2) * lambda * std::pow(2, ((lambda - 1) / 2));
    double sigma = std::pow((num / den), (1 / lambda));

    std::normal_distribution<double> u_n(0, sigma);
    std::normal_distribution<double> v_n(0, 1.0);

    for(int i = 0; i < nVehicles; i++){
        double u = u_n(gen);
        double v = v_n(gen);
        double step = u / std::pow(std::abs(v), (1 / lambda));
        newCuckoo.solution[i] += levyStepSize * step;
    }

    return newCuckoo;
}

//de jong function
double MCS::fitness(solution_t solution){
    double fitness = 0;

    for(int i = 0; i < solution.size(); i++){
        fitness += std::pow(solution[i], 2);
    }

    return fitness;
}

// custo de transporte por unidade de distancia, multiplicado pela distancia
// c * dist + H * V_utilizados
double MCS::fitness2(v vehicles, int usedVehicles){
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

void MCS::initPopulation(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::cout << "Initializing Population" << std::endl;

    for (int i = 0; i < nNests; i++){
        cuckoo newCuckoo;
        // newCuckoo.vehicles = Vehicle::instantiateVehicles(nVehicles, 1, 100);
        for(int j = 0; j < nVehicles; j++){
            newCuckoo.solution.push_back(lowerBound[j] + (upperBound[j] - lowerBound[j]) * dis(gen));
        }
        nests.push_back(newCuckoo);
    }
    std::cout << "Population Initialized" << std::endl;
    return;
}

list_t constructSupplierCandidateList(Instance* inst, Vehicle v, std::vector<int>& availableSuppliers, int removedSupplier = -1){

    list_t candidateList;
    double heuristic = 0;

    //ranking candidate node k to be inserted between nodes i and j
    // delta dist = dist(i,k) + dist(k,j) - dist(i,j)
    double deltaDist = 0;
    double deltaCost = 0;

    //at maximum, should use demandPerRetailer.size() vehicles
    
    if(removedSupplier != -1){
        std::vector<int>::iterator it = std::find(availableSuppliers.begin(), availableSuppliers.end(), removedSupplier);
        availableSuppliers.erase(it);
    }

    std::vector<int>routes = v.getRoutes();
    for(int i = 0; i< availableSuppliers.size();i++){
        for(int j = 0; j < routes.size()-1;j++){
            deltaDist = inst->retailerCrossDockDist[routes[j]][availableSuppliers[i]] + inst->retailerCrossDockDist[availableSuppliers[i]][routes[j+1]] - inst->retailerCrossDockDist[routes[j]][routes[j+1]];
            deltaCost = inst->c * deltaDist + inst->COST;
            heuristic = deltaCost/deltaDist;
            candidateList.push_back(std::make_tuple(heuristic, availableSuppliers[i], routes[j], routes[j+1]));
        }
    }
    
    return candidateList;

}

list_t constructRetailerCandidateList(Instance* inst, Vehicle v, std::vector<int>& availableRetailers, int removedRetailer = -1){

    list_t candidateList;
    double heuristic = 0;

    // ranking candidate node k to be inserted between nodes i and j
    // delta dist = dist(i,k) + dist(k,j) - dist(i,j)
    double deltaDist = 0;
    double deltaCost = 0;
    
    if(removedRetailer != -1){
        std::vector<int>::iterator it = std::find(availableRetailers.begin(), availableRetailers.end(), removedRetailer);
        availableRetailers.erase(it);
    }

    std::vector<int>routes = v.getRoutes();
    for(int i = 0; i< availableRetailers.size();i++){
        for(int j = 0; j < routes.size()-1;j++){
            deltaDist = inst->retailerCrossDockDist[routes[j]][availableRetailers[i]] + inst->retailerCrossDockDist[availableRetailers[i]][routes[j+1]] - inst->retailerCrossDockDist[routes[j]][routes[j+1]];
            deltaCost = inst->c * deltaDist + inst->COST;
            heuristic = deltaCost/deltaDist;
            candidateList.push_back(std::make_tuple(heuristic, availableRetailers[i], routes[j], routes[j+1]));
        }
    }
    
    return candidateList;

}

void MCS::supplierInit(cuckoo& cuckoo){
    std::vector<int>availableSuppliers;
    
    std::vector<std::pair<double, bool>> demandPerProduct = this->inst->demandPerProduct;

    for (int i = 1; i <= this->inst->nSuppliers; i++){
        availableSuppliers.push_back(i);
    }

    int k = 0;
    int attended = 0;
    int l = 0, m = 0, o = 0;

    while (cuckoo.usedVehicles <= this->nVehicles){
        
        if(demandPerProduct.size() == attended){
            break;
        }

        list_t candidateList = constructSupplierCandidateList(this->inst, cuckoo.vehicles[k], availableSuppliers);
        std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

        cuckoo.vehicles[k].setType(SUPPLIER);

        for(std::pair<double,bool> &demand : demandPerProduct){
            
            if(demand.second){
                continue;
            }

            l = std::get<1>(candidateList.front());
            m = std::get<2>(candidateList.front());
            o = std::get<3>(candidateList.front());
            
            double capacity = cuckoo.vehicles[k].getCapacity() - demand.first;

            if(capacity < 0 && demand.first == inst->demandPerProduct.back().first){
                capacity = 0;
                break;
            } else if(capacity < 0){
                continue;
            }
            std::cout << "Inserting between " << m << " and " << o << " supplier " << l << std::endl;
            cuckoo.vehicles[k].insertBetween(m,o,l);
            // candidateList.erase(candidateList.begin());
            cuckoo.vehicles[k].setCapacity(capacity);

            demand.second = true;
            attended++;
            
            std::cout << "Attended: " << attended << std::endl;
            std::cout << "Demand size: " << demandPerProduct.size() << std::endl;

            if(demandPerProduct.size() == attended){
                break;
            }
            if(cuckoo.vehicles[k].getCapacity() == 0){
                break;
            }
            std::cout <<"Removing supplier " << l << " from available suppliers" << std::endl;
            candidateList = constructSupplierCandidateList(this->inst, cuckoo.vehicles[k], availableSuppliers, l);
            std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});
            
        }
    
        cuckoo.usedVehicles++;
        k++;
    }
}

void MCS::retailerInit(cuckoo& cuckoo){
    std::vector<int>availableRetailers;
    
    std::vector<std::pair<double, bool>> demandPerRetailer = this->inst->demandPerRetailer;
    std::vector<std::pair<double,bool>> returnedPerRetailer = this->inst->returnedPerRetailer;

    for(int i = 1; i <= this->inst->nRetailers; i++){
        availableRetailers.push_back(i);
    }

    // print available retailers
    std::cout << "Available retailers: ";
    for(int i = 0; i < availableRetailers.size(); i++){
        std::cout << availableRetailers[i] << " ";
    }

    for (int i = 0; i < this->inst->nRetailers; i++) {
        demandPerRetailer[i].second = false;
    }

    // print demand per retailer
    std::cout << std::endl << "Demand per retailer: ";
    for(int i = 0; i < demandPerRetailer.size(); i++){
        std::cout << demandPerRetailer[i].first << " ";
    }

    // print returned per retailer
    std::cout << std::endl << "Returned per retailer: ";
    for(int i = 0; i < returnedPerRetailer.size(); i++){
        std::cout << returnedPerRetailer[i].first << " ";
    }

    int k = cuckoo.usedVehicles;
    int attended = 0;
    int l = 0, m = 0, o = 0;

    while(k <= this->nVehicles){
        
        if(demandPerRetailer.size() == attended && returnedPerRetailer.size() == attended){
            break;
        }

        list_t candidateList = constructRetailerCandidateList(this->inst, cuckoo.vehicles[k], availableRetailers);

        std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});

        cuckoo.vehicles[k].setType(RETAILER);

        for (std::pair<double,bool> &demand : demandPerRetailer){
            
            if (demand.second) continue;

            l = std::get<1>(candidateList.front());
            m = std::get<2>(candidateList.front());
            o = std::get<3>(candidateList.front());
            
            double capacity = cuckoo.vehicles[k].getCapacity() - demand.first + returnedPerRetailer[attended].first;

            if(capacity < 0 && demand.first == inst->demandPerRetailer.back().first){
                capacity = 0;
                break;
            } else if(capacity < 0){
                continue;
            }

            std::cout << "Inserting between " << m << " and " << o << " retailer " << l << std::endl;

            cuckoo.vehicles[k].insertBetween(m,o,l);
            cuckoo.vehicles[k].setCapacity(capacity);

            demand.second = true;
            returnedPerRetailer[attended].second = true;
            attended++;
            
            std::cout << "Attended: " << attended << std::endl;
            std::cout << "Demand size: " << demandPerRetailer.size() << std::endl;
            std::cout << "Returned size: " << returnedPerRetailer.size() << std::endl;

            if(demandPerRetailer.size() == attended && returnedPerRetailer.size() == attended){
                break;
            }

            if(cuckoo.vehicles[k].getCapacity() == 0){
                break;
            }

            std::cout <<"Removing retailer " << l << " from available retailers" << std::endl;

            candidateList = constructSupplierCandidateList(this->inst, cuckoo.vehicles[k], availableRetailers, l);
            std::sort(candidateList.begin(), candidateList.end(), [](candidate &a, candidate &b) {return std::get<0>(a) < std::get<0>(b);});
            
        }
    
        cuckoo.usedVehicles++;
        k++;
    }
}

void MCS::initPopulation2(){

    // generate initial nNests solutions with nVehicles vehicles
    for (int i = 0; i < nNests; i++){
        cuckoo newCuckoo;
        newCuckoo.vehicles = Vehicle::instantiateVehicles(nVehicles, this->inst->COST, this->inst->capacity);

        // init suppliers routes
        supplierInit(newCuckoo);

        std::cout << std::endl << "---------------------------" << std::endl;
        std::cout << "Vehicles used for suppliers: " << newCuckoo.usedVehicles << std::endl;
        std::cout << "---------------------------" << std::endl << std::endl;

        // init retailers routes
        retailerInit(newCuckoo);

        nests.push_back(newCuckoo);
    } 

    for(auto nest : nests){
        for(auto vehicle : nest.vehicles){
            std::cout << vehicle << std::endl;
        }
    }

    exit(1);
}

void MCS::search(){

    std::cout << "Initializing Search" << std::endl;

    // initPopulation();
    initPopulation2();
    std::random_device rd;
    //initial fitness
    std::cout << "Calculating initial fitness" << std::endl;
    for (int i = 0; i < nNests; i++)
    {
        nests[i].fitness = fitness(nests[i].solution);
    }

    int generation = 1;
    std::cout << "Starting Search" << std::endl;
    for(int i =0; i < this->nIterations; i++){
        //choose a random nest
        generation++;
        std::sort(nests.begin(), nests.end(), [](cuckoo &a, cuckoo &b) {return a.fitness < b.fitness;});
        std::cout << "Iteration: " << i + 1 << " Best Fitness: " << nests[0].fitness << std::endl;

        int s = (int) (pa * nNests);
        
        for (int j = nNests - s; j < nNests; j++)
        {
            levyStepSize = maxLevyStepSize/(std::sqrt(generation));
           
            cuckoo newCuckoo = this->newCuckoo(j);
            double newFitness = fitness(newCuckoo.solution);
            
            nests[j].solution = newCuckoo.solution;
            nests[j].fitness = newFitness;
            
        }

        std::uniform_int_distribution<> dis(0, nNests - s);

        for(int j = 0; j < nNests - s; j++){
            
            int randomNest = dis(rd);
            if(randomNest == j){
                levyStepSize = maxLevyStepSize/(std::pow(generation, 2));
                
                cuckoo newCuckoo = this->newCuckoo(randomNest);
                double newFitness = fitness(newCuckoo.solution);
                int randomNest2 = dis(rd);
                if(newFitness < nests[randomNest2].fitness){
                    nests[randomNest2].solution = newCuckoo.solution;
                    nests[randomNest2].fitness = newFitness;
                }
            } else {
                cuckoo cuckooK;
                
                for(int k = 0; k < nVehicles; k++){
                    cuckooK.solution.push_back((std::abs(nests[j].solution[k] - nests[randomNest].solution[k]))/phi);
                }
                cuckooK.fitness = fitness(cuckooK.solution);
                int randomNest2 = dis(rd);

                if(cuckooK.fitness < nests[randomNest2].fitness){
                    nests[randomNest2].solution = cuckooK.solution;
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
        std::cout << nests[0].solution[i] << " ";
    }
    return;
}

MCS::~MCS(){
    nests.clear();
    return;
}