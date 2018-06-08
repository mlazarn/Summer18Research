#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/thirdLargeRun/";

    Lattice testLattice(path, 1028);
    
    cout << "LatticeSize=" << sizeof(testLattice);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(1600000000, 1000000, 800000000);

    cout << "test complete" << endl;
    return 0;
}
