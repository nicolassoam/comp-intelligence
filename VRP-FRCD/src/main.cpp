#include "../include/utils.hpp"
#include "../include/config.hpp"

int main(int argc, char** arvv){
    std::vector<std::string> files = Util::readDirectory(INPUT);
    std::cout << files[0] << std::endl;
    Util::readInstance(files[0]);
}