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
#include "instance.hpp"
#include "cuckoo.hpp"

namespace fs = std::filesystem;

namespace Util
{
    using namespace std;

    bool compareNat(const std::string& a, const std::string& b)
    {
        if (a.empty())
            return true;
        if (b.empty())
            return false;
        if (std::isdigit(a[0]) && !std::isdigit(b[0]))
            return true;
        if (!std::isdigit(a[0]) && std::isdigit(b[0]))
            return false;
        if (!std::isdigit(a[0]) && !std::isdigit(b[0]))
        {
            if (std::toupper(a[0]) == std::toupper(b[0]))
                return compareNat(a.substr(1), b.substr(1));
            return (std::toupper(a[0]) < std::toupper(b[0]));
        }

        // Both strings begin with digit --> parse both numbers
        std::istringstream issa(a);
        std::istringstream issb(b);
        int ia, ib;
        issa >> ia;
        issb >> ib;
        if (ia != ib)
            return ia < ib;

        // Numbers are the same --> remove numbers and recurse
        std::string anew, bnew;
        std::getline(issa, anew);
        std::getline(issb, bnew);
        return (compareNat(anew, bnew));
    }

    vector<string> readDirectory(string path)
    {
        vector<string> files;
        for (const auto &entry : fs::directory_iterator(path))
        {
            files.push_back(entry.path().string());
        }

        sort(files.begin(), files.end(), compareNat);

        return files;
    }

    void initVectors(int num, vector<vector<double>> &vec) {
        for (auto& inner : vec) {
            inner.resize(num, 0);
        }
    }

    void saveSolution(string file, cuckoo best){
        ofstream solution;
        solution.open(file, ios::out);
        solution << "Vehicles: " <<std::endl;
        for(auto v : best.vehicles){
            solution << v << endl;
            
        }
        solution << "Fitness: " << best.fitness << endl;
        solution << best.fitness << endl;
    }

    Instance* readInstance(string file)
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

        int nSuppliers = 0, nRetailers = 0, nOutlets = 0, nVehicles = 0, tMax = 0, COST = 0, capacity = 0, c = 0;

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

        supplierCrossDockDist.resize(nSuppliers + 1);
        initVectors(nSuppliers+1, supplierCrossDockDist);

        supplierCrossDockTime.resize(nSuppliers + 1, vector<double>(nSuppliers + 1));
        initVectors(nSuppliers+1, supplierCrossDockTime);
        
        retailerCrossDockDist.resize(nRetailers + 1, vector<double>(nRetailers + 1)); 
        initVectors(nRetailers+1, retailerCrossDockDist);
        
        retailerCrossDockTime.resize(nRetailers + 1, vector<double>(nRetailers + 1));
        initVectors(nRetailers+1, retailerCrossDockTime);
        
        retailerProductDemand.resize(nRetailers, vector<double>(nSuppliers));
        initVectors(nSuppliers, retailerProductDemand);
        
        returnedProductRetailer.resize(nRetailers, vector<double>(nSuppliers));
        initVectors(nSuppliers, returnedProductRetailer);
        
        outletCrossDockDist.resize(nOutlets + 1, vector<double>(nOutlets + 1));
        initVectors(nOutlets+1, outletCrossDockDist);
        
        outletCrossDockTime.resize(nOutlets + 1, vector<double>(nOutlets + 1));
        initVectors(nOutlets+1, outletCrossDockTime);
        
        outletProductDemand.resize(nOutlets, vector<double>(nSuppliers));
        initVectors(nSuppliers, outletProductDemand);
        
        returnedProductOutlet.resize(nOutlets, vector<double>(nSuppliers));
        initVectors(nSuppliers, returnedProductOutlet);
        
        defectiveProduct.resize(nSuppliers,0);

        std::vector<vector<double>> tempVec;
        
        while (getline(instance, line))
        {
            // if (line.empty())
            //     continue;

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
        int k = 0 ;

        //e'[i][j]
        for (int i = lim; i < nSuppliers+1; i++)
        {
            for (int j = 0; j < nSuppliers+1; j++)
            {
                supplierCrossDockDist[i][j] = tempVec[i][j];
            }
        }

        lim = nSuppliers+2;
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

        lim = lim +nRetailers+2;
        k = 0;

        //e'''[i][j]
        for (int i = lim; i < nOutlets+1 + lim; i++)
        {
            for (int j = 0; j < nOutlets+1; j++)
            {
                outletCrossDockDist[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim + nOutlets+2;
        k = 0;

        //t'[i][j]
        for (int i = lim; i < nSuppliers+1 + lim; i++)
        {
            for (int j = 0; j < nSuppliers+1; j++)
            {
                supplierCrossDockTime[k][j] = tempVec[i][j];
            }
            k++;
        }

        lim = lim + nSuppliers+2;
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

        lim = lim + nRetailers+2;
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

        lim = lim+ nOutlets+2;
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

        lim = lim + nRetailers+1;
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
        lim = lim + nRetailers+1;
        
        for (int i = lim; i < lim +1; i++)
        {
            for(int j = 0; j < nSuppliers; j++)
            {
                defectiveProduct[j] = tempVec[i][j];
            }
        }

        //d'''[i][k]
        lim = lim +2;
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
        lim = lim + nOutlets+1;
        k = 0;
        for (int i = lim; i < nOutlets + lim; i++)
        {
            for(int j = 0; j < nSuppliers; j++)
            {
                returnedProductOutlet[k][j] = tempVec[i][j];
            }
            k++;
        }

        instance.close();
        Instance* inst = new Instance(nSuppliers, nRetailers, nOutlets, nVehicles, tMax, COST, capacity, c,
                                     supplierCrossDockDist, supplierCrossDockTime,
                                     retailerCrossDockDist, retailerCrossDockTime,
                                     outletCrossDockDist, outletCrossDockTime,
                                     retailerProductDemand, returnedProductRetailer,
                                     outletProductDemand, returnedProductOutlet,
                                     defectiveProduct);
        return inst;
    }

}

#endif // UTILS_HPP_