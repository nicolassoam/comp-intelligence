#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <filesystem>

namespace fs = std::filesystem;

namespace Util{

    using namespace std;

    vector<string> readDirectory(string path){
        vector<string> files;
        for (const auto & entry : fs::directory_iterator(path)){
            files.push_back(entry.path().string());
        }

        sort(files.begin(), files.end(),[](const string& a, const string& b){
            return a.length() < b.length();});

        return files;
    }

    void readInstance(){
        int nSuppliers, nRetailers, nOutlets, nVehicles, tMax, COST, capacity = 0;
        
    }
}

#endif // UTILS_HPP_