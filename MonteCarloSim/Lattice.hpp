#include "Cell.hpp"

#include <boost/random/mersenne_twister.hpp> 
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <sstream>
#include <vector>

class Lattice
{
    private:
        static const int size = 1028;
        static Cell latt[size][size];

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
        Lattice();
        Lattice(string path);
        ~Lattice() = default;
        void reaction(int x, int y);
        void dataOutput();
        void reactTest();
        void monteCarloRun(int steps, int interval, int startRecord);
};

