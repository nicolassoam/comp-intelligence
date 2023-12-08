#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/vehicle.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** argv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::cout << files[0] << std::endl;
    
    Instance *inst = Util::readInstance(files[0]);

    int nEggs = 10;
    int nNests = 3;
    int nIterations = 1000;

    std::vector<double> lowerBound(nEggs, -5.12);
    std::vector<double> upperBound(nEggs, 5.12);
    MCS* mcs = new MCS(nNests, nEggs, nIterations, lowerBound , upperBound, inst);
    mcs->search();

    return 0;
}