#include "Cell.h"

Cell::Cell()
{
    predation = 0.5;
    fertility = 0.5;
    diffusion = 0.5;
    pairSwap = 0.5;

    species = 3;
}

void Cell::set(int param, double value, int spec)
{
    species = spec;

    if (param >= 0 && param < 4 && value >= 0.0)
    {
        switch(param)
        {
            case PRED   : predation = value; break;
            case FERT   : fertility = value; break;
            case DIFF   : diffusion = value; break;
            case PSWP   : pairSwap = value; break;
        }
    }
}

void Cell::setSpecies(int newSpec) {species = newSpec;}
