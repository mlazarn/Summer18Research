#include "Lattice.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
<<<<<<< HEAD
    std::string path = "firstLargeRun/";

    cout << "Initializing lattice: " << endl;
=======
    string path = "data/thirdMonteCarloRun/";
>>>>>>> 94a8a10cc8cd8bdfb10b271398051800ebc5b7ca

    Lattice testLattice(path, 256);
    
    cout << "LatticeSize=" << sizeof(testLattice);

    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
<<<<<<< HEAD
    //testLattice.monteCarloRun(1000000000, 1000000, 50000000);
=======
    testLattice.monteCarloRun(800000000, 1000000, 50000000);
>>>>>>> 94a8a10cc8cd8bdfb10b271398051800ebc5b7ca

    cout << "test complete" << endl;
    return 0;
}
