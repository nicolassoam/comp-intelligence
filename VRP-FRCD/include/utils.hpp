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

namespace Util
{

    using namespace std;

    vector<string> readDirectory(string path)
    {
        vector<string> files;
        for (const auto &entry : fs::directory_iterator(path))
        {
            files.push_back(entry.path().string());
        }

        sort(files.begin(), files.end(), [](const string &a, const string &b)
             { return a < b; });

        return files;
    }

    void readInstance(string file)
    {

        ifstream instance;

        try
        {
            instance.open(file, ios::in);
            if (!instance.is_open())
                throw runtime_error("Could not open file");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            exit(1);
        }

        int nSuppliers, nRetailers, nOutlets, nVehicles, tMax, COST, capacity, c = 0;

        // e'[i][j]
        vector<vector<double>> supplierCrossDockDist;

        // t'[i][j]
        vector<vector<double>> supplierCrossDockTime;

        // e''[i][j]
        vector<vector<double>> retailerCrossDockDist;

        // t''[i][j]
        vector<vector<double>> retailerCrossDockTime;

        // d''[i][k]
        vector<vector<double>> retailerProductDemand;

        // r''[i][k]
        vector<vector<double>> returnedProductRetailer;

        // e'''[i][j]
        vector<vector<double>> outletCrossDockDist;

        // t'''[i][j]
        vector<vector<double>> outletCrossDockTime;

        // d'''[i][k]
        vector<vector<double>> outletProductDemand;

        // r'''[i][k]
        vector<vector<double>> returnedProductOutlet;

        // p[k]
        vector<double> defectiveProduct;

        string line;

        // first 8 lines
        int i = 0;

        while (getline(instance, line) && i < 8)
        {
            string value = " ";
            value = line.substr(line.find("=") + 1, line.length());

            switch (i)
            {
            case 0:
                nSuppliers = stoi(value);
                break;
            case 1:
                nRetailers = stoi(value);
                break;
            case 2:
                nOutlets = stoi(value);
                break;
            case 3:
                nVehicles = stoi(value);
                break;
            case 4:
                capacity = stoi(value);
                break;
            case 5:
                c = stoi(value);
                break;
            case 6:
                tMax = stoi(value);
                break;
            case 7:
                COST = stoi(value);
                break;
            }

            i++;
        }

        line = " ";
        i = 0;
        int j = 0;

        supplierCrossDockDist.resize(nSuppliers+1, vector<double>(nSuppliers+1));
        supplierCrossDockTime.resize(nSuppliers+1, vector<double>(nSuppliers+1));
        retailerCrossDockDist.resize(nRetailers+1), vector<double>(nRetailers+1);
        retailerCrossDockTime.resize(nRetailers+1, vector<double>(nRetailers+1));
        retailerProductDemand.resize(nRetailers+1, vector<double>(nSuppliers+1));
        returnedProductRetailer.resize(nRetailers+1, vector<double>(nSuppliers+1));
        outletCrossDockDist.resize(nOutlets+1, vector<double>(nOutlets+1));
        outletCrossDockTime.resize(nOutlets + 1, vector<double>(nOutlets + 1));
        outletProductDemand.resize(nOutlets + 1, vector<double>(nRetailers + 1));
        returnedProductOutlet.resize(nOutlets + 1, vector<double>(nRetailers + 1));
        defectiveProduct.resize(nSuppliers);

        while(getline(instance,line)){
            if(line == "e'[i][j]"){
                getline(instance,line);
                stringstream ss(line);
                string token;
                j == 0;
                while(getline(ss,token,' ')){
                    supplierCrossDockDist[i][j] = stod(token);
                    j++;
                }
                i++;
            }
        }

        // // 2D matrices
        // vector<vector<double>> matrices[] = {
        //     supplierCrossDockDist,
        //     supplierCrossDockTime,
        //     retailerCrossDockDist,
        //     retailerCrossDockTime,
        //     retailerProductDemand,
        //     returnedProductRetailer,
        //     outletCrossDockDist,
        //     outletCrossDockTime,
        //     outletProductDemand,
        //     returnedProductOutlet};

        // for (auto &matrix : matrices)
        // {
        //     getline(instance, line); // skip the line with the matrix name
        //     cout << line << endl;
        //     for (int i = 0; i < matrix.size(); ++i)
        //     {
        //         for (int j = 0; j < matrix[i].size(); ++j)
        //         {
        //             if (!(instance >> matrix[i][j]))
        //             {
        //                 throw runtime_error("Error reading matrix data");
        //             }
        //         }
        //     }
        // }

        // // vector for defective products
        // getline(instance, line); // skip the line with the vector name
        // // cout << line << endl;
        // for (int i = 0; i < defectiveProduct.size(); ++i)
        // {
        //     if (!(instance >> defectiveProduct[i]))
        //     {
        //         throw runtime_error("Error reading vector data");
        //     }
        // }
        // cout << matrices[0][0][0]   << endl;
    }
}

#endif // UTILS_HPP_