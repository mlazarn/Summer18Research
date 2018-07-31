#include "Cell.hpp"
#include "Lattice.hpp"

#include <boost/random/mersenne_twister.hpp> 
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <fftw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <sstream>
#include <vector>

class LatticeMLRPS : public Lattice
{
    protected:
        int RPSMin;
        int RPSMax;
        int interfaceDistance;
        int orientation;
        double mobilityRateRPS;
        int topology; // Indicates the topology of the RPS Patch.
                      // 0 indicates a square patch on the face of the torus.
                      //   The edge of which is interfaceDistance away from the
                      //   boundary of the lattice
                      //   i.e.:
                      //   ________________
                      //   |              |
                      //   |              |
                      //   |    ++++++    |
                      //   |    ++++++    |
                      //   |    ++++++    |
                      //   |    ++++++    |
                      //   |              |
                      //   |______________|
                      // 1 indicates an annular region that wraps along the x
                      //   direction of the torus. The ring starts at y=0 and ends
                      //   at y=interfaceDistance.
                      //   i.e.:
                      //   ________________________________________
                      //   |++++++|                               |
                      //   |++++++|                               |
                      //   |++++++|                               |
                      //   |++++++|                               |
                      //   |++++++|                               |
                      //   |++++++|_______________________________|

        virtual void metadata(int start, int interval, int stop);
        void metadata(int start, int interval, int stop, int swap, int swapInterval);
        void metadata(int start, int interval, int stop, int startDrive, int driveFrequency, int pulseWidth);

    public:
        LatticeMLRPS();
        LatticeMLRPS(string path, int orr, int lattSize, double mobility, double mobilityRPS, int intDist, int binSize);
        LatticeMLRPS(string path, int orr, int patchTop, int xSize, int ySize, double mobility, double mobilityRPS, int intDist, int binSize);
        //~LatticeMLRPS();

        void RPSReaction(int x, int y);
        virtual void monteCarloRun(int steps, int interval, int start);
        virtual void monteCarloRun(int steps, int interval, int start, int swap, int swapInterval);
        virtual void drivenMonteCarloRun(int steps, int interval, int start, int startDrive, int driveFrequency, int pulseWidth);
        virtual void specAnalysisRun(int steps, int interval, int startRecord);
};
