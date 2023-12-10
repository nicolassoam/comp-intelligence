#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    
    Instance *inst = Util::readInstance(files[0]);
    int nVehicles = inst->nVehicles;
    int nNests = 4;
    int nIterations = 1000;

    std::vector<double> lowerBound(nVehicles, -5.12);
    std::vector<double> upperBound(nVehicles, 5.12);
    MCS* mcs = new MCS(nNests, nVehicles, nIterations, lowerBound , upperBound, inst);
    mcs->search();

    return 0;
}