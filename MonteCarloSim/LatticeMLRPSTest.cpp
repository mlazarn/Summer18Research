#include "LatticeMLRPS.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    string path = "data/diffusionTest/";

    for (int i = 0; i < 100; i++)
    {
        stringstream ss;

        ss << path << "test_" << i << "/";

        string fullPath = ss.str();

        stringstream sss;
        sss << "mkdir -p " << fullPath;

        string command = sss.str();

        const int dir_err = system(command.c_str());
        if (dir_err == -1)
        {
            printf("Error creating directory!n");
            exit(1);
        }

        double swapRate = 1.0 + (i / 10.0);
        
        cout << "swapRate = " << swapRate << endl;

        LatticeMLRPS testLattice(path, 1028, swapRate, 384, 640);

        cout << "beginning test" << endl;

        testLattice.dataOutput();

        //testLattice.reactTest();
        testLattice.monteCarloRun(300000000, 1000000, 100000000, 200000000, 100000);

        cout << "test complete" << endl;
    }
    

    
    return 0;
}
