#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    
    Instance *inst = Util::readInstance(files[32]);
    int nVehicles = inst->nVehicles;
    int nNests = 10;
    int nIterations = 1000;

    MCS* mcs = new MCS(nNests, nVehicles, nIterations, inst);
    mcs->search();

    return 0;
}