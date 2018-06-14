#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/fourthWipeTest/";

    Lattice testLattice(path, 1024);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(1600000000, 1000000, 500000000, 900000000, 509, 516);

    cout << "test complete" << endl;
    return 0;
}
