#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = string(argv[1]).append("/");
    int size = atoi(argv[2]);
    double mobility = atof(argv[3]);
    int binSize = 4;
    int steps = atoi(argv[4]);
    int interval = atoi(argv[5]);
    int start = atoi(argv[6]);

    Lattice testLattice(path, size, mobility, binSize);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloDensityRun(steps, interval, start);

    cout << "test complete" << endl;
    return 0;
}
