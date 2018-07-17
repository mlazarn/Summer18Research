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
        int sizeX;
        int sizeY;
        Cell ** latt;

        double mobilityRate;

        int aPop;
        int bPop;
        int cPop;
        
        double **flux;
        double **density0;
        double **density1;

        int binWidth;
        int binnedArraySize;
        double *binMidpoints;
        int **binnedDeathCounts;
        int **binnedBirthCounts;
        int **binnedDiffusionCounts;
        double **binnedFlux;
        double **binnedDensity0;
        double **binnedDensity1;

        int timestep;
        int monteCarloStep;

        string filePath;

        boost::random::mt19937 rng;
        boost::random::uniform_int_distribution<> xCoordDist;
        boost::random::uniform_int_distribution<> yCoordDist;
        boost::random::uniform_int_distribution<> neighDist;
        boost::random::uniform_01<> actionDist;

        void incrementSpeciesCount(int spec);
        void decrementSpeciesCount(int spec);

        void updateDensity();
        void updateFlux();
        void updateBinnedReactionCount(int reaction, int species, int y);
        void clearBinnedReactionCount();
        void updateBinnedDensity();
        void updateBinnedFlux();
        //void updateCurrent(int spec, int x, int current);
        //void clearCurrent();
        
        void initializeArrays();
        void initializeLattice();
        
        virtual void metadata(int start, int interval, int stop);
        virtual void progressBar(float progress);
    
    public:
        Lattice();
        Lattice(string path, int lattSize, double mobility, int binSize);
        Lattice(string path, int xSize, int ySize, double mobility, int binSize);

        virtual ~Lattice();
        void reaction(int x, int y);
        void binMidpointsOutput();
        void dataOutput();
        void setControlRow(int row);
        void setControlCol(int col);
        virtual void monteCarloRun(int steps, int interval, int start);
};

