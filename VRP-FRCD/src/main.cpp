#include "../include/utils.hpp"
#include "../include/config.hpp"
#include "../include/cuckoo.hpp"
#include <numbers>

int main(int argc, char** arvv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::cout << files[0] << std::endl;
    Instance *inst = Util::readInstance(files[1]);
    std::vector<double> lowerBound(100, -5.12);
    std::vector<double> upperBound(100, 5.12);
    MCS* mcs = new MCS(100, 50, 500, lowerBound , upperBound);
    mcs->search();

    return 0;
}