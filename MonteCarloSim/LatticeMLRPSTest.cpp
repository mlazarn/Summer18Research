#include "LatticeMLRPS.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    string path = "data/densityTest/";

    LatticeMLRPS testLattice(path, 1, 256, 5.0, 64);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(700000000, 1000000, 100000000, 400000000, 1000000);

    cout << "test complete" << endl;

    return 0;
}
