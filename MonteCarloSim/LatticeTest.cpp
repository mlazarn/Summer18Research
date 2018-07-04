#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/newDataTest2/";

    Lattice testLattice(path, 200, 0.8);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(600000000, 1000000, 100000000);

    cout << "test complete" << endl;
    return 0;
}
