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

        enum Parameters{PRED, FERT, DIFF, PSWP};

        int species;

    public:
        Cell();
        void set(int param, double value, int spec);
        //note: n is the number of neighbors.

        double getPredRate() const      {return predation;}
        void setPredRate(double rate)   {predation = rate;}

        double getFertRate() const      {return fertility;}
        void setFertRate(double rate)   {fertility = rate;}

        double getDifRate() const       {return diffusion;}
        void setDifRate(double rate)    {diffusion = rate;}

        double getSwapRate() const      {return pairSwap;}
        void setSwapRate(double rate)   {pairSwap = rate;}

        int getSpecies() const       {return species;}
        void setSpecies(int newSpec);
};

#endif /* ifndef CELL_H */

