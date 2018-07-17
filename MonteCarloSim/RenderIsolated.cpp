#include "LatticeMLRPS.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    string pathML = string(argv[1]).append("/");
    string pathRPS = string(argv[2]).append("/");
    int orientation = atoi(argv[3]);
    int size = atoi(argv[4]);
    double mobility = atof(argv[5]);
    double mobilityRPS = mobility;
    int binSize = 8;
   
    int intDistML =0;
    int intDistRPS = size - 1;

    //cout << "," << path << "," << orientation << "," << size << ", " << mobility << ", " << interfaceDistance << endl; // ", " << steps << ", " << interval << ", " << start << ", " << swapTime << ", " << swapInterval << endl;
    

    //LatticeMLRPS testLattice(path, orientation, xSize, mobility, mobilityRPS, interfaceDistance);
    //cout << "initializing lattice" << endl;
    LatticeMLRPS latticeML(pathML, orientation, size, mobility, mobilityRPS, intDistML, binSize);
    LatticeMLRPS latticeRPS(pathRPS, orientation, size, mobility, mobilityRPS, intDistRPS, binSize);

    cout << "beginning test" << endl;
    
    int steps = atoi(argv[6]);
    int interval = atoi(argv[7]);
    int start = atoi(argv[8]);

    cout << "starting run" << endl;

    latticeML.monteCarloRun(steps, interval, start);
    latticeRPS.monteCarloRun(steps, interval, start);

    cout << "test complete" << endl;
    
    return 0;
}
