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

        supplierCrossDockDist.resize(nSuppliers + 1, vector<double>(nSuppliers + 1));
        supplierCrossDockTime.resize(nSuppliers + 1, vector<double>(nSuppliers + 1));
        retailerCrossDockDist.resize(nRetailers + 1, vector<double>(nRetailers + 1));
        retailerCrossDockTime.resize(nRetailers + 1, vector<double>(nRetailers + 1));
        retailerProductDemand.resize(nRetailers, vector<double>(nSuppliers));
        returnedProductRetailer.resize(nRetailers, vector<double>(nSuppliers));
        outletCrossDockDist.resize(nOutlets + 1, vector<double>(nOutlets + 1));
        outletCrossDockTime.resize(nOutlets + 1, vector<double>(nOutlets + 1));
        outletProductDemand.resize(nOutlets, vector<double>(nSuppliers));
        returnedProductOutlet.resize(nOutlets, vector<double>(nSuppliers));
        defectiveProduct.resize(nSuppliers);
        std::vector<vector<double>> tempVec;
        while (getline(instance, line))
        {
            if (line.empty())
                continue;
            if (line.find("=") != string::npos) // Skip lines with matrix names
            {
                getline(instance, line); // Move to the next line for matrix values
            }
            vector<double> aux;
            // Create a stringstream from the line
            std::istringstream ss(line);

            // Read the values from the stringstream into a temporary vector

            double val;
            while (ss >> val)
            {
                aux.push_back(val);
            }

            tempVec.push_back(aux);
        }

        //transfer values to the correct matrix
        int lim = 0;
        int k =0 ;

        //e'[i][j]
        for (int i = lim; i < nSuppliers+1; i++)
        {
            for (int j = 0; j < nSuppliers+1; j++)
            {
                supplierCrossDockDist[i][j] = tempVec[i][j];
            }
        }

        lim = nSuppliers+1;
        k = 0;

        //e''[i][j]
        for (int i = lim; i < nRetailers+1 + lim; i++)
        {
            for (int j = 0; j < nRetailers+1; j++)
            {
                retailerCrossDockDist[k][j] = tempVec[i][j];
                
            }
            k++;
        }

        lim = lim +nRetailers+1;
        k= 0;
        //e'''[i][j]
        for (int i = lim; i < nOutlets+1 + lim; i++)
        {
            for (int j = 0; j < nOutlets+1; j++)
            {
                outletCrossDockDist[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim =lim + nOutlets+1;
        k= 0;
        //t'[i][j]
        for (int i = lim; i < nSuppliers+1 + lim; i++)
        {
            for (int j = 0; j < nSuppliers+1; j++)
            {
                supplierCrossDockTime[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim + nSuppliers+1;
        k = 0;
        //t''[i][j]
        for (int i = lim; i < nRetailers+1 + lim; i++)
        {
            for (int j = 0; j < nRetailers+1; j++)
            {
                retailerCrossDockTime[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim + nRetailers+1;
        k = 0;
        //t'''[i][j]
        for (int i = lim; i < nOutlets+1 + lim; i++)
        {
            for (int j = 0; j < nOutlets+1; j++)
            {
                outletCrossDockTime[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim+ nOutlets+1;
        k = 0;
        //d''[i][k]
        for (int i = lim; i < nRetailers + lim; i++)
        {
            for (int j = 0; j < nSuppliers; j++)
            {
                retailerProductDemand[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim + nRetailers;
        k = 0;
        //r''[i][k]
        for (int i = lim; i < nRetailers + lim; i++)
        {
            for (int j = 0; j < nSuppliers; j++)
            {
                returnedProductRetailer[k][j] = tempVec[i][j];
            }
            k++;
        }

        //p[k]
        lim = lim + nRetailers;
        
        for (int i = lim; i < lim +1; i++)
        {
            for(int j = 0; j < nSuppliers; j++)
            {
                defectiveProduct[j] = tempVec[i][j];
            }
        }

        //d'''[i][k]
        lim = lim +1;
        k = 0;
        for (int i = lim; i < nOutlets + lim; i++)
        {
            for(int j = 0; j < nSuppliers; j++)
            {
                outletProductDemand[k][j] = tempVec[i][j];
            }
            k++;
        }

        //r'''[i][k]
        lim = lim + nOutlets;
        k = 0;
        for (int i = lim; i < nOutlets + lim; i++)
        {
            for(int j = 0; j < nSuppliers; j++)
            {
                returnedProductOutlet[k][j] = tempVec[i][j];
            }
            k++;
        }

        for(auto i : returnedProductOutlet){
            for(auto j : i){
                cout << j << " ";
            }
            cout << endl;
        }

        // for(auto i: defectiveProduct){
        //     cout << i << " ";
        // }


    }

}

#endif // UTILS_HPP_