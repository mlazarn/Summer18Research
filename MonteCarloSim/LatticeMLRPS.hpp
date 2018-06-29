#include "Cell.hpp"
#include "Lattice.hpp"

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

class LatticeMLRPS : public Lattice
{
    protected:
        int RPSMin;
        int RPSMax;
        int interfaceDistance;
        int orientation;
        int topology;

        virtual void metadata(int start, int interval, int stop);
        void metadata(int start, int stop, int interval, int swap, int swapInterval);

    public:
        LatticeMLRPS();
        LatticeMLRPS(string path, int orientation, int lattSize, double mobility, int interfaceDistance);
        LatticeMLRPS(string path, int orientation, int topology, int xSize, int ySize, double mobility, int interfaceDistance);
        ~LatticeMLRPS() = default;

        void RPSReaction(int x, int y);
        virtual void monteCarloRun(int steps, int interval, int start);
        virtual void monteCarloRun(int steps, int interval, int start, int swap, int swapInterval);
};
