#include "Cell.hpp"
#include "Lattice.hpp"

#include <boost/random/mersenne_twister.hpp> 
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
//#include <fftw3.h>

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
        
        // Calculates the vertical autocorrelator (C_{spec spec} for some 
        // distance r and some column y.
        // @param int mode      1 if subtract density square, 0 otherwise
        // @param int symmetry  1 if <a(x, y).a(x+r, y> should be calculated in
        //                      both directions, 0 if it should only be calculated
        //                      in the positive direction.
        double autoCorrelator(int spec, int y, int r, int mode, int symmetry);

        // Calculates and writes to a file the autocorrelation function C(r).
        //
        // @param int spec      The species for which the autocorrelation function 
        //                      is calculated. Use spec=3 to calculate the 
        //                      autocorrelation averaged over all three species.
        //
        // @param int subMode   1 if subtract density square, 0 otherwise. Is 
        //                      passed directly to autoCorrelator(spec, y, r, mode)
        //                      as the value for mode.
        // @param int symmetry  1 if <a(x, y).a(x+r, y> should be calculated in
        //                      both directions, 0 if it should only be calculated
        //                      in the positive direction.
        void outputAutoCorr(int spec, int subMode, int symmetry);

        void RPSReaction(int x, int y);
        virtual void monteCarloRun(int steps, int interval, int start);
        virtual void monteCarloRun(int steps, int interval, int start, int swap, int swapInterval);
        virtual void drivenMonteCarloRun(int steps, int interval, int start, int startDrive, int driveFrequency, int pulseWidth);
        virtual void specAnalysisRun(int steps, int interval, int startRecord, int run);
};
