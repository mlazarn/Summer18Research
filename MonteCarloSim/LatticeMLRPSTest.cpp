#include "LatticeMLRPS.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    //cout << argc << endl;
    //cout << "argv[1]=" << argv[1] << ", argv[2]=" << argv[2] << ", argv[3]=" << argv[3] << ", argv[4]=" << argv[4] << ", argv[5]=" << argv[5];
    //cout << ", argv[6]=" << argv[6] << ", argv[7]=" << argv[7] << ", argv[8]=" << argv[8] << ", argv[9]=" << argv[9] << ", argv[10]=" << argv[10] << endl;

    //cout << "\n\n Now testing converting the arguments";

    string path = string(argv[1]).append("/");
    int orientation = atoi(argv[2]);
    int topology = atoi(argv[3]);
    int xSize = atoi(argv[4]);
    int ySize = atoi(argv[5]);
    double mobility = atof(argv[6]);
    int interfaceDistance = atoi(argv[7]);

    //cout << "," << path << "," << orientation << "," << size << ", " << mobility << ", " << interfaceDistance << endl; // ", " << steps << ", " << interval << ", " << start << ", " << swapTime << ", " << swapInterval << endl;
    

    LatticeMLRPS testLattice(path, orientation, topology, xSize, ySize, mobility, interfaceDistance);

    cout << "beginning test" << endl;

    //testLattice.dataOutput();

    int steps = atoi(argv[8]);
    int interval = atoi(argv[9]);
    int start = atoi(argv[10]);
    int startDrive = atoi(argv[11]);
    int driveFrequency = atoi(argv[12]);
    int pulseWidth = atoi(argv[13]);

    //int swapTime = atoi(argv[11]);
    //int swapInterval = atoi(argv[12]);

    //int steps = atoi(argv[6]);
    //int interval = atoi(argv[7]);
    //int start = atoi(argv[8]);
    //int swapTime = atoi(argv[9]);
    //int swapInterval = atoi(argv[10]);

    testLattice.drivenMonteCarloRun(steps, interval, start, startDrive, driveFrequency, pulseWidth);
    //testLattice.monteCarloRun(steps, interval, start, swapTime, swapInterval);

    cout << "test complete" << endl;

    //cout << path << ", " << orientation << ", " << topology << ", " << xSize << ", " << ySize << ", " << mobility << ", " << interfaceDistance << ", " << steps << ", " << interval << ", " << start << endl; // ", " << swapTime << ", " << swapInterval << endl;
    return 0;
}
