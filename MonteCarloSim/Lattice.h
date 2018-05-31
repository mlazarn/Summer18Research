#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <chrono>
#include <sstream>

#include <boost/random/mersenne_twister.hpp> 
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>

#include <vector>
#include "Cell.h"

using namespace boost;

class Lattice
{
    private:
        static const int size = 100;
        Cell latt[size][size];

        int aPop;
        int bPop;
        int cPop;

        int timestep;

        string filePath;

        boost::random::mt19937 rng;
        boost::random::uniform_int_distribution<> coordDist;
        boost::random::uniform_int_distribution<> neighDist;
        boost::random::uniform_01<> actionDist;

        void incrementSpeciesCount(int spec);
        void decrementSpeciesCount(int spec);
    
    public:
        Lattice(double * popDist);
        ~Lattice();
        void reaction(int x, int y);
        void dataOutput();
        void monteCarloRun(int steps, int interval, int startRecord);
};

