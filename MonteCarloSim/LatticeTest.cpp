#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::string path = "firstLargeRun/";

    cout << "Initializing lattice: " << endl;

    Lattice testLattice(path);
    
    cout << "LatticeSize=" << sizeof(testLattice);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    //testLattice.monteCarloRun(1000000000, 1000000, 50000000);

    cout << "test complete" << endl;
    return 0;
}
