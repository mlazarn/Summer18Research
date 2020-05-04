#include "LatticeMLRPS.hpp"
//#include <cnpy.h>

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    //cout << argc << endl;
    //cout << "path=" << argv[1] << ", orientation=" << argv[2] << ", topology=" << argv[3] << ", xSize=" << argv[4] << ", ySize=" << argv[5];
    //cout << ", mobility=" << argv[6] << ", mobilityRPS=" << argv[7] << ", interfaceDistance=" << argv[8] << ", binSize=" << argv[9] << ", steps=" << argv[10] << ", interval=" << argv[11];
    //cout << ", start=" << argv[12] << ", run=" << argv[13] << endl;

    //cout << "\n\n Now testing converting the arguments";

    string path = string(argv[1]).append("/");
    int orientation = atoi(argv[2]);
    //int size = atoi(argv[3]);
    /*
    double mobility = atof(argv[4]);
    double mobilityRPS = atof(argv[5]);
    int interfaceDistance = atoi(argv[6]);
    */
    
    int topology = atoi(argv[3]);
    int xSize = atoi(argv[4]);
    int ySize = atoi(argv[5]);
    double mobility = atof(argv[6]);
    double mobilityRPS = atof(argv[7]);
    int interfaceDistance = atoi(argv[8]);
    int binSize = atoi(argv[9]);
   

    //cout << "," << path << "," << orientation << "," << size << ", " << mobility << ", " << interfaceDistance << endl; // ", " << steps << ", " << interval << ", " << start << ", " << swapTime << ", " << swapInterval << endl;
    

    //LatticeMLRPS testLattice(path, orientation, xSize, mobility, mobilityRPS, interfaceDistance);
    cout << "initializing lattice" << endl;
    LatticeMLRPS testLattice(path, orientation, topology, xSize, ySize, mobility, mobilityRPS, interfaceDistance, binSize);

    //cout << "beginning test" << endl;

    //testLattice.dataOutput();
    
    /*
    int steps = atoi(argv[7]);
    int interval = atoi(argv[8]);
    int start = atoi(argv[9]);
    */
    
    
    int steps = atoi(argv[10]);
    int interval = atoi(argv[11]);
    int start = atoi(argv[12]);
    int run = atoi(argv[13]);
    /*
    int startDrive = atoi(argv[12]);
    int driveFrequency = atoi(argv[13]);
    int pulseWidth = atoi(argv[14]);
    */
    

    //int swapTime = atoi(argv[11]);
    //int swapInterval = atoi(argv[12]);

    //int steps = atoi(argv[6]);
    //int interval = atoi(argv[7]);
    //int start = atoi(argv[8]);
    //int swapTime = atoi(argv[9]);
    //int swapInterval = atoi(argv[10]);

    //cout << path << ", " << orientation << "< " << topology << "< " << xSize;
    //cout << "< " << ySize << "< " << mobility << "< " << mobilityRPS << "< ";
    //cout << interfaceDistance << "< " << steps << "< " << interval << "< " << start << endl;
    cout << "starting run" << endl;
    //testLattice.binMidpointsOutput();
    testLattice.specAnalysisRun(steps, interval, start, run);
    //testLattice.monteCarloRun(steps, interval, start, swapTime, swapInterval);
    //testLattice.drivenMonteCarloRun(steps, interval, start, startDrive, driveFrequency, pulseWidth);

    cout << "test complete" << endl;
    return 0;

    //cout << path << ", " << orientation << ", " << topology << ", " << xSize << ", " << ySize << ", " << mobility << ", " << interfaceDistance << ", " << steps << ", " << interval << ", " << start << endl; // ", " << swapTime << ", " << swapInterval << endl;
}
