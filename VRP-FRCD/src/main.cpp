#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::cout << files[0] << std::endl;
    Instance *inst = Util::readInstance(files[1]);
    std::vector<Vehicle> vehicles = Vehicle::instantiateVehicles(inst->nVehicles, inst->COST, inst->capacity);

    std::cout << vehicles[9] << std::endl;

    int nEggs = 2;
    int nNests = 100;
    int nIterations = 1000;

    // std::vector<double> lowerBound(nEggs, -100);
    // std::vector<double> upperBound(nEggs, 100);
    // MCS* mcs = new MCS(nNests, nEggs, nIterations, lowerBound , upperBound);
    // mcs->search();

    return 0;
}