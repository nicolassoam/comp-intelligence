#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>
#include "omp.h"

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double> elapsed;
    for(int j =0 ;j <5; j++){
        #pragma omp parallel for
        for(int i = 0; i < files.size(); i++){
            Instance *inst = Util::readInstance(files[i]);
            int nVehicles = inst->nVehicles;
            int nNests = inst->nSuppliers;
            int nIterations = 2000;
            start = std::chrono::high_resolution_clock::now();
            MCS* mcs = new MCS(nNests, nVehicles, nIterations, inst);
            mcs->search();
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            cuckoo b = mcs->getBestCuckoo();
            std::string output = OUTPUT + files[i].substr(files[i].find_last_of('/') + 1);
            delete mcs;
            delete inst;
            Util::saveSolution(output, b, elapsed.count());
        }
    }

    return 0;
}