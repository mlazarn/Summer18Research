#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/secondWallTest/";

    Lattice testLattice(path, 1024);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(1600000000, 1000000, 500000000, 900000000, 505, 520);

    cout << "test complete" << endl;
    return 0;
}
