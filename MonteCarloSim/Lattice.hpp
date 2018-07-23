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
        // Declare lattice dimensions and lattice array
        int sizeX;
        int sizeY;
        Cell ** latt;

        // May-Leonard mobility rate. This gets passed to the cell initiatiors.
        double mobilityRate;

        // Tracks population of each species
        int aPop;
        int bPop;
        int cPop;
        
        // Declare arrays to keep track of flux & density
        double **flux;
        double **density0;
        double **density1;

        // Declare arrays and necessary values for binned statistics.
        int binWidth;
        int binnedArraySize;
        double *binMidpoints;
        int **binnedDeathCounts;
        int **binnedBirthCounts;
        int **binnedDiffusionCounts;
        double **binnedFlux;
        double **binnedDensity0;
        double **binnedDensity1;

        // Keep track of time through the simulation.
        int timestep;
        int monteCarloStep;

        // where the outputs go.
        string filePath;

        // random number generator and distributions
        boost::random::mt19937 rng;
        boost::random::uniform_int_distribution<> xCoordDist;
        boost::random::uniform_int_distribution<> yCoordDist;
        boost::random::uniform_int_distribution<> neighDist;
        boost::random::uniform_01<> actionDist;

        void incrementSpeciesCount(int spec);
        void decrementSpeciesCount(int spec);

        // data tracking functions.
        double globalDensity();
        void updateDensity();
        void updateFlux();
        void updateBinnedReactionCount(int reaction, int species, int y);
        void clearBinnedReactionCount();
        void updateBinnedDensity();
        void updateBinnedFlux();
        
        // initializers that get used in construction methods. 
        void initializeArrays();
        void initializeLattice();
        
        // data writing.
        virtual void metadata(int start, int interval, int stop);
        virtual void progressBar(float progress);
    
    public:
        //constructors
        Lattice();
        Lattice(string path, int lattSize, double mobility, int binSize);
        Lattice(string path, int xSize, int ySize, double mobility, int binSize);

        //destructor
        virtual ~Lattice();

        //implements the reaction. 
        void reaction(int x, int y);

        //writes an array that just gives some spacial coordinates for the binned data.
        void binMidpointsOutput();
        void dataOutput();
        void setControlRow(int row);
        void setControlCol(int col);
        virtual void monteCarloRun(int steps, int interval, int start);
        virtual void monteCarloDensityRun(int steps, int interval, int start);
};

