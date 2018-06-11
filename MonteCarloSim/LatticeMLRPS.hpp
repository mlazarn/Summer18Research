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

    public:
        LatticeMLRPS();
        LatticeMLRPS(string path, int lattSize, int min, int max);
        ~LatticeMLRPS() = default;

        void RPSReaction(int x, int y);
        virtual void monteCarloRun(int steps, int interval, int startRecord);
};
