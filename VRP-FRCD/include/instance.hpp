#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include <iostream>
#include <vector>

using matrix = std::vector<std::vector<double>>;
using vector_d = std::vector<double>;

enum availability {
    AVAILABLE,
    UNAVAILABLE,
    VISITED
};

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

    // demandPerRetailer
    std::vector<std::pair<double,availability>> demandPerRetailer;

    // demandPerProduct
    std::vector<std::pair<double,availability>> demandPerProduct;

    // demandPerRetailer
    std::vector<std::pair<double,availability>> returnedPerRetailer;

    // outletDemand
    std::vector<std::pair<double,availability>> outletDemand;

    // returnedOutlet
    std::vector<std::pair<double,availability>> returnedOutlet;

    vector_d supplierReturnedProducts;

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

    void calculateProductDemand();                  // returns total quantity of each product
    void calculateTotalDemandPerRetailer();         // returns total demand of each retailer
    void calculateReturnedProductsPerRetailer();    // returns quantity of returned products per retailer
    void calculateOutletDemand();                   // returns total demand of each outlet
    void calculateReturnedProductsPerOutlet();      // returns quantity of returned products per outlet
    void calculateReturnedSupplierProducts();       // returns quantity of returned products per supplier

    ~Instance();

};




#endif // INSTANCE_HPP_