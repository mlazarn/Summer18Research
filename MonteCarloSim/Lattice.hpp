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
    protected:
        int size;
        Cell ** latt;

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
        Lattice(string path, int lattSize);
        virtual ~Lattice();
        void reaction(int x, int y);
        void dataOutput();
        void reactTest();
        virtual void monteCarloRun(int steps, int interval, int startRecord);
        void monteCarloRun(int steps, int interval, int startRecord, int wipe, int wipeMin, int wipeMax);
};

