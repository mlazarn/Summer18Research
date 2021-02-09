#ifndef CELL_H
#define CELL_H 

using namespace std;

class Cell
{
    private:
        double predation;
        double fertility;
        double diffusion;
        double pairSwap;
        double predationRPS;
        double diffusionRPS;

        int species;

    public:
        /*
         * Default constructor. Sets the species to 3 (dead cell) and 
         * sets the parameters to reasonable defaults. The actual values don't
         * matter though as each parameter will be set when initializing the 
         * lattice later.
         */
        Cell();

        /*
         * A set of accessors and mutators for all of the parameters.
         */
        double getPredRate() const      {return predation;}
        void setPredRate(double rate)   {predation = rate;}

        double getFertRate() const      {return fertility;}
        void setFertRate(double rate)   {fertility = rate;}

        double getDifRate() const       {return diffusion;}
        void setDifRate(double rate)    {diffusion = rate;}

        double getSwapRate() const      {return pairSwap;}
        void setSwapRate(double rate)   {pairSwap = rate;}

        double getRPSPredation() const  {return predationRPS;}
        void setRPSPredation(double pred) {predationRPS = pred;}

        double getRPSDiffusion() const {return diffusionRPS;}
        void setRPSDiffusion(double diff) {diffusionRPS = diff;}

        int getSpecies() const       {return species;}
        void setSpecies(int newSpec);
};

#endif /* ifndef CELL_H */

