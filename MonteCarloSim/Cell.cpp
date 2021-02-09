#include "Cell.hpp"


Cell::Cell()
{
    predation = 1.0;
    predationRPS = 1.0;
    fertility = 1.0;
    diffusion = 5.0;
    diffusionRPS = 5.0;
    pairSwap = 5.0;

    species = 3;
}

void Cell::setSpecies(int newSpec) {species = newSpec;}
