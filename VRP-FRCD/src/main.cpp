#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** arvv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::cout << files[0] << std::endl;
    Instance *inst = Util::readInstance(files[1]);

    int nEggs = 100;
    int nNests = 100;
    int nIterations = 1000;
    std::vector<double> lowerBound(nEggs, -600);
    std::vector<double> upperBound(nEggs, 600);
    MCS* mcs = new MCS(nNests, nEggs, nIterations, lowerBound , upperBound);
    mcs->search();

    return 0;
}