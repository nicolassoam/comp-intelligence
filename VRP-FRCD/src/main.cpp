#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    for(auto f : files){
        Instance *inst = Util::readInstance(f);
        int nVehicles = inst->nVehicles;
        int nNests = 4;
        int nIterations = 500;

        MCS* mcs = new MCS(nNests, nVehicles, nIterations, inst);
        mcs->search();
        cuckoo b = mcs->getBestCuckoo();
        std::string output = OUTPUT + f.substr(f.find_last_of('/') + 1);
        delete mcs;
        delete inst;
        Util::saveSolution(output, b);
    }
    

    return 0;
}