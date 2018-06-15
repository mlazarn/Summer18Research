#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/newAlgTest/";

    Lattice testLattice(path, 512, 5.0);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(800000000, 1000000, 100000000);

    cout << "test complete" << endl;
    return 0;
}
