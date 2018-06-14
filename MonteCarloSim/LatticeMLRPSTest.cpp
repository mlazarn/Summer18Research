#include "LatticeMLRPS.hpp"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    string path = "data/fourthSwapTest/";

    LatticeMLRPS testLattice(path, 1028, 256, 772);
    
    cout << "beginning test" << endl;

    testLattice.dataOutput();

    //testLattice.reactTest();
    testLattice.monteCarloRun(300000000, 1000000, 100000000, 200000000, 100000);

    cout << "test complete" << endl;
    return 0;
}
