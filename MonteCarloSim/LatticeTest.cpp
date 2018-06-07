#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/thirdMonteCarloRun/";

    Lattice testLattice(path, 256);
    
    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(800000000, 1000000, 50000000);

    cout << "test complete" << endl;
    return 0;
}
