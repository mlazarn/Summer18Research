#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <chrono>
#include <sstream>

#include <boost/random/uniform_real.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/mersenne_twister.hpp> 

#include <boost/random/variate_generator.hpp>
#include <sys/time>
#include <vector>

using namespace boost

class Lattice
{
    private:
        int *latt;
        enum species;
        int *population;
        int latticeSize;
    
    public:
        Lattice(int size, int *popDensity);
        ~Lattice();
        void reaction();
        void monteCarloRun();
        void main();
};

Lattice::Lattice(int size, int *popDensity)
{
    int newLattice[size][size];
    latt = &newLattice;

    
}
