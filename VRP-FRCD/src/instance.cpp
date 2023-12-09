#include "../include/instance.hpp"

Instance::Instance(){
    nSuppliers = nRetailers = nOutlets = nVehicles = tMax = COST = capacity = c = 0;
}

Instance::~Instance(){
    supplierCrossDockDist.clear();
    supplierCrossDockTime.clear();
    retailerCrossDockDist.clear();
    retailerCrossDockTime.clear();
    outletCrossDockDist.clear();
    outletCrossDockTime.clear();
    retailerProductDemand.clear();
    returnedProductRetailer.clear();
    outletProductDemand.clear();
    returnedProductOutlet.clear();
    defectiveProduct.clear();
}

Instance::Instance(int nSuppliers, int nRetailers, int nOutlets, int nVehicles, int tMax, int COST, int capacity, int c){
    this->nSuppliers = nSuppliers;
    this->nRetailers = nRetailers;
    this->nOutlets = nOutlets;
    this->nVehicles = nVehicles;
    this->tMax = tMax;
    this->COST = COST;
    this->capacity = capacity;
    this->c = c;
}

Instance::Instance(int nSuppliers, int nRetailers, int nOutlets, int nVehicles, 
                   int tMax, int COST, int capacity, int c,
                   matrix supplierCrossDockDist, matrix supplierCrossDockTime,
                   matrix retailerCrossDockDist, matrix retailerCrossDockTime,
                   matrix outletCrossDockDist, matrix outletCrossDockTime,
                   matrix retailerProductDemand, matrix returnedProductRetailer,
                   matrix outletProductDemand, matrix returnedProductOutlet,
                   vector_d defectiveProduct){
                    
    this->nSuppliers = nSuppliers;
    this->nRetailers = nRetailers;
    this->nOutlets = nOutlets;
    this->nVehicles = nVehicles;
    this->tMax = tMax;
    this->COST = COST;
    this->capacity = capacity;
    this->c = c;
    this->supplierCrossDockDist = supplierCrossDockDist;
    this->supplierCrossDockTime = supplierCrossDockTime;
    this->retailerCrossDockDist = retailerCrossDockDist;
    this->retailerCrossDockTime = retailerCrossDockTime;
    this->outletCrossDockDist = outletCrossDockDist;
    this->outletCrossDockTime = outletCrossDockTime;
    this->retailerProductDemand = retailerProductDemand;
    this->returnedProductRetailer = returnedProductRetailer;
    this->outletProductDemand = outletProductDemand;
    this->returnedProductOutlet = returnedProductOutlet;
    this->defectiveProduct = defectiveProduct;
    calculateProductDemand();
    calculateTotalDemandPerRetailer();
    calculateReturnedProductsPerRetailer();
}

void Instance::calculateProductDemand(){
    
    for (int i = 0; i < nSuppliers; i++){
        int sum = 0;

        for(int j = 0; j < nRetailers; j++){
            sum += retailerProductDemand[j][i];
        }

        this->demandPerProduct.push_back({sum,false});
    }
}

void Instance::calculateTotalDemandPerRetailer(){
    
    for(int i = 0; i < nRetailers; i++){
        int sum = 0;

        for (int j = 0; j < nSuppliers; j++){
            sum += retailerProductDemand[i][j];
        }

        this->demandPerRetailer.push_back({sum,false});
    }
}

void Instance::calculateReturnedProductsPerRetailer(){
    
    for(int i = 0; i < nRetailers; i++){
        double sum = 0;

        for (int j = 0; j < nSuppliers; j++){
            sum += returnedProductRetailer[i][j];
        }

        this->returnedPerRetailer.push_back({sum,false});
    }
}

void Instance::printBasicInfo(){
    std::cout << "nSuppliers: " << nSuppliers << std::endl;
    std::cout << "nRetailers: " << nRetailers << std::endl;
    std::cout << "nOutlets: " << nOutlets << std::endl;
    std::cout << "nVehicles: " << nVehicles << std::endl;
    std::cout << "tMax: " << tMax << std::endl;
    std::cout << "COST: " << COST << std::endl;
    std::cout << "capacity: " << capacity << std::endl;
    std::cout << "c: " << c << std::endl;
}

void Instance::printSupplierInfo(){
    std::cout << "supplierCrossDockDist: " << std::endl;
    for(int i = 0; i < nSuppliers+1; i++){
        for(int j = 0; j < nSuppliers+1; j++){
            std::cout << supplierCrossDockDist[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "supplierCrossDockTime: " << std::endl;
    for(int i = 0; i < nSuppliers+1; i++){
        for(int j = 0; j < nSuppliers+1; j++){
            std::cout << supplierCrossDockTime[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Instance::printRetailerInfo(){
    std::cout << "retailerCrossDockDist: " << std::endl;
    for(int i = 0; i < nRetailers+1; i++){
        for(int j = 0; j < nRetailers+1; j++){
            std::cout << retailerCrossDockDist[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "retailerCrossDockTime: " << std::endl;
    for(int i = 0; i < nRetailers+1; i++){
        for(int j = 0; j < nRetailers+1; j++){
            std::cout << retailerCrossDockTime[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "retailerProductDemand: " << std::endl;
    for(int i = 0; i < nRetailers; i++){
        for(int j = 0; j < nSuppliers; j++){
            std::cout << retailerProductDemand[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "returnedProductRetailer: " << std::endl;
    for(int i = 0; i < nRetailers; i++){
        for(int j = 0; j < nSuppliers; j++){
            std::cout << returnedProductRetailer[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Instance::printOutletInfo(){
    std::cout << "outletCrossDockDist: " << std::endl;
    for(int i = 0; i < nOutlets+1; i++){
        for(int j = 0; j < nOutlets+1; j++){
            std::cout << outletCrossDockDist[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "outletCrossDockTime: " << std::endl;
    for(int i = 0; i < nOutlets+1; i++){
        for(int j = 0; j < nOutlets+1; j++){
            std::cout << outletCrossDockTime[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "outletProductDemand: " << std::endl;
    for(int i = 0; i < nOutlets; i++){
        for(int j = 0; j < nSuppliers; j++){
            std::cout << outletProductDemand[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "returnedProductOutlet: " << std::endl;
    for(int i = 0; i < nOutlets; i++){
        for(int j = 0; j < nSuppliers; j++){
            std::cout << returnedProductOutlet[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

