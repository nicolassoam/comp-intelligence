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

    void readInstance(string file){

        ifstream instance;

        try
        {
            instance.open(file, ios::in);
            if(!instance.is_open())
                throw runtime_error("Could not open file");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        int nSuppliers, nRetailers, nOutlets, nVehicles, tMax, COST, capacity = 0;

        //e'[i][j]
        vector<vector<double>>supplierCrossDockDist;

        //t'[i][j]
        vector<vector<double>>supplierCrossDockTime;

        //e''[i][j]
        vector<vector<double>>retailerCrossDockDist;

        //t''[i][j]
        vector<vector<double>>retailerCrossDockTime;

        //d''[i][k]
        vector<vector<double>>retailerProductDemand;

        //r''[i][k]
        vector<vector<double>>returnedProductRetailer;

        //e'''[i][j]
        vector<vector<double>>outletCrossDockDist;

        //t'''[i][j]
        vector<vector<double>>outletCrossDockTime;

        //d'''[i][k]
        vector<vector<double>>outletProductDemand;

        //r'''[i][k]
        vector<vector<double>>returnedProductOutlet;

        //p[k]
        vector<double>defectiveProduct;
    }
}

#endif // UTILS_HPP_