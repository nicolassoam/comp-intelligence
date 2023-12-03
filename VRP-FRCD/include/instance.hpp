#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include <iostream>
#include <vector>

using matrix = std::vector<std::vector<double>>;
using vector_d = std::vector<double>;

struct Instance {

    int nSuppliers, nRetailers, nOutlets, nVehicles, tMax, COST, capacity, c;

    // e'[i][j]
    matrix supplierCrossDockDist;

    // t'[i][j]
    matrix supplierCrossDockTime;

    // e''[i][j]
    matrix retailerCrossDockDist;

    // t''[i][j]
    matrix retailerCrossDockTime;
    
    // e'''[i][j]
    matrix outletCrossDockDist;

    // t'''[i][j]
    matrix outletCrossDockTime;

    // d''[i][k]
    matrix retailerProductDemand;

    // r''[i][k]
    matrix returnedProductRetailer;

    // d'''[i][k]
    matrix outletProductDemand;

    // r'''[i][k]
    matrix returnedProductOutlet;

    // p[k]
    vector_d defectiveProduct;

    // Default constructor
    Instance();

    // Constructor
    Instance(int nSuppliers, int nRetailers, int nOutlets, int nVehicles, int tMax, int COST, int capacity, int c);

    // Constructor
    Instance(int nSuppliers, int nRetailers, int nOutlets, int nVehicles, 
             int tMax, int COST, int capacity, int c,
             matrix supplierCrossDockDist, matrix supplierCrossDockTime,
             matrix retailerCrossDockDist, matrix retailerCrossDockTime,
             matrix outletCrossDockDist, matrix outletCrossDockTime,
             matrix retailerProductDemand, matrix returnedProductRetailer,
             matrix outletProductDemand, matrix returnedProductOutlet,
             vector_d defectiveProduct);
    
    void printBasicInfo();
    void printSupplierInfo();
    void printRetailerInfo();
    void printOutletInfo();

    ~Instance();

};




#endif // INSTANCE_HPP_