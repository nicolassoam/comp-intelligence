#include "../include/cuckoo.hpp"


MCS::MCS( int nNests, int nEggs, int nIterations, solution_t lowerBound, solution_t upperBound){
    this->nNests = nNests;
    this->nEggs = nEggs;
    this->nIterations = nIterations;
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->levyStepSize = maxLevyStepSize;
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

    for(int i = 0; i < newCuckoo.solution.size(); i++){
        double u = u_n(gen);
        double v = v_n(gen);
        double step = u / std::pow(std::abs(v), (1 / lambda));
        newCuckoo.solution[i] += levyStepSize * step;
    }

    return newCuckoo;
}

//rastringin function
double MCS::fitness(solution_t solution){
    double fitness = 0;
    
    for(int i = 0; i < solution.size(); i++){
        fitness += std::pow(solution[i], 2) - 10 * std::cos(2 * std::numbers::pi * solution[i]) + 10;
    }
    return fitness;
}

void MCS::initPopulation(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::cout << "Initializing Population" << std::endl;

    for(int i = 0; i < nNests; i++){
        cuckoo newCuckoo;
        
        for(int j = 0; j < nEggs; j++){
            newCuckoo.solution.push_back(lowerBound[j] + (upperBound[j] - lowerBound[j]) * dis(gen));
        }
        nests.push_back(newCuckoo);
    }
    std::cout << "Population Initialized" << std::endl;
    return;
}

void MCS::search(){

    std::cout << "Initializing Search" << std::endl;

    initPopulation();
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
        std::cout << "Generation: " << generation << " Best Fitness: " << nests[0].fitness << std::endl;

        int s = (int) (pa * nNests);
        
        for (int j = nNests - s; j < nNests; j++)
        {
            levyStepSize = maxLevyStepSize/(std::sqrt(generation));
           
            cuckoo newCuckoo = this->newCuckoo(j);
            double newFitness = fitness(newCuckoo.solution);
            if(newFitness < nests[j].fitness){
                nests[j].solution = newCuckoo.solution;
                nests[j].fitness = newFitness;
            }
        }
        
        for(int j = 0; j < nNests - s; j++){
            std::uniform_int_distribution<> dis(0, nNests - s);
            
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
                double dx = 0;
                cuckoo cuckooK;
                
                for(int k = 0; k < nests[j].solution.size(); k++){
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
    std::cout << "Best Solution: " << std::endl;
    for(int i = 0; i < nests[0].solution.size(); i++){
        std::cout << nests[0].solution[i] << " ";
    }
    return;
}