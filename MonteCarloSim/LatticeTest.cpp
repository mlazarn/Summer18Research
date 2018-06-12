#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/firstWipeTest/";

    Lattice testLattice(path, 1028);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(1600000000, 1000000, 800000000, 256, 768);

    cout << "test complete" << endl;
    return 0;
}
