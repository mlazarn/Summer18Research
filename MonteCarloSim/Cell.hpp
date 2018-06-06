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

        double getPredRate() const   {return predation;}
        double getFertRate() const   {return fertility;}
        double getDifRate() const    {return diffusion;}
        double getSwapRate() const   {return pairSwap;}

        int getSpecies() const       {return species;}
        void setSpecies(int newSpec);
};

#endif /* ifndef CELL_H */

