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

        double mobilityRate;

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
        virtual void metadata(int start, int interval, int stop);
        virtual void progressBar(float progress);
    
    public:
        Lattice();
        Lattice(string path, int lattSize, double mobility);
        virtual ~Lattice();
        void reaction(int x, int y);
        void dataOutput();
        void reactTest();
        void setControlRow(int row);
        void setControlCol(int col);
        virtual void monteCarloRun(int steps, int interval, int start);
        void monteCarloRun(int steps, int interval, int start, int change, int interfaceDistance);
};

