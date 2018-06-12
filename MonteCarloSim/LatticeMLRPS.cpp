#include "LatticeMLRPS.hpp"

using namespace std;

LatticeMLRPS::LatticeMLRPS() : Lattice()
{
    RPSMin = 64;
    RPSMax = 192;
}

LatticeMLRPS::LatticeMLRPS(string path, int latticeSize, int min, int max) : Lattice(path, latticeSize)
{
    RPSMin = min;
    RPSMax = max;
}

/*
LatticeMLRPS::~LatticeMLRPS()
{
    cout << "~LatticeMLRPS()" << endl;
    for (int i = 0; i < size; ++i) 
    {
        delete[] latt[i];
    }

    delete[] latt;
}
*/

void LatticeMLRPS::RPSReaction(int x, int y)
{
    int neigh = neighDist(rng);

    int X = x, Y = y;
    
    switch(neigh)
    {
        case 0 : //up
            if (x == 0)
            {
                X = size - 1;
            }
            else
            {
                X = (x - 1) % size;
            }
            break;
        case 1 : //right
            Y = (y + 1) % size;
            break;
        case 2 : //down
            X = (x + 1) % size;
            break;
        case 3 : //left
            if (y == 0)
            {
                Y = size - 1;
            }
            else
            {
                Y = (y - 1) % size;
            }
            break;
    }

    
    Cell & curr = latt[x][y];
    Cell & neighbor = latt[X][Y];
    double rand = actionDist(rng);

    //cout << "(" << x << "," << y << ")" << "(" << X << "," << Y << ")" << endl;

    //Empty Neighbor
    if (neighbor.getSpecies() == 3)
    {
        if (rand <= curr.getDifRate())
        {
            neighbor.setSpecies(curr.getSpecies());
            curr.setSpecies(3);
        }          
    }
    // Prey Neighbor
    else if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
    {
        //Predation
        if (rand <= curr.getPredRate() / 2)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            incrementSpeciesCount(curr.getSpecies());
            neighbor.setSpecies(curr.getSpecies());
        }
        //Pair Swapping
        else if (rand > curr.getPredRate() / 2 && rand <= (curr.getPredRate() + curr.getSwapRate()) / 2)
        {
            int tmp = neighbor.getSpecies();
            neighbor.setSpecies(curr.getSpecies());
            curr.setSpecies(tmp);
        }
    }
    // Predator Neighbor
    else if (neighbor.getSpecies() == (curr.getSpecies() - 1) % 3 || (neighbor.getSpecies() == 2 && curr.getSpecies() == 0) )
    {
        //Predation
        if (rand <= neighbor.getPredRate() / 2)
        {
            decrementSpeciesCount(curr.getSpecies());
            incrementSpeciesCount(neighbor.getSpecies());
            curr.setSpecies(neighbor.getSpecies());
        }
        //Pair Swapping
        else if (rand > neighbor.getPredRate() / 2 && rand <= (neighbor.getPredRate() + neighbor.getSwapRate()) / 2)
        {
            int tmp = neighbor.getSpecies();
            neighbor.setSpecies(curr.getSpecies());
            curr.setSpecies(tmp);
        }
    }
}

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord)
{
    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        int x = coordDist(rng);
        int y = coordDist(rng);

        do 
        {
            x = coordDist(rng);
            y = coordDist(rng);
        }
        while (latt[x][y].getSpecies() > 2);
 
        if ( (x >= RPSMin && x <= RPSMax) && (y >= RPSMin && y <= RPSMax)) 
        {
            RPSReaction(x, y);
        }
        else
        {
            reaction(x, y);
        }

        if (timestep % interval == 0)
        {
            cout << timestep << endl;
            if (timestep >= startRecord)
            {
                dataOutput();
            }
        }

        timestep++;
    }
    while (timestep <= steps);
}

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord, int swapTime, int postSwapInterval)
{
    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        int x = coordDist(rng);
        int y = coordDist(rng);

        do 
        {
            x = coordDist(rng);
            y = coordDist(rng);
        }
        while (latt[x][y].getSpecies() > 2);
 
        if ( (x >= RPSMin && x <= RPSMax) && (y >= RPSMin && y <= RPSMax) && timestep >= swapTime) 
        {
            RPSReaction(x, y);
        }
        else
        {
            reaction(x, y);
        }

        if (timestep < swapTime && timestep % interval == 0)
        {
            cout << timestep << endl;
            if (timestep >= startRecord)
            {
                dataOutput();
            }
        }
        else if (timestep % postSwapInterval == 0)
        {
            cout << timestep << endl;
            dataOutput();
        }
        timestep++;
    }
    while (timestep <= steps);
    /*cout << "Starting Monte Carlo Run" << endl;
    do
    {
        int x = coordDist(rng);
        int y = coordDist(rng);

        do 
        {
            x = coordDist(rng);
            y = coordDist(rng);
        }
        while (latt[x][y].getSpecies() > 2);
 
        if (timestep < swapTime)
        {
            reaction(x, y);

            if (timestep % interval == 0)
            {
                cout << timestep << endl;
                if (timestep >= startRecord)
                {
                    dataOutput();
                }
            }
        }
        else
        {
            RPSReaction(x, y);

            if (timestep % postSwapInterval == 0)
            {
                cout << timestep << endl;
                if (timestep >= startRecord)
                {
                    dataOutput();
                }
            }
        }


        timestep++;
    }
    while (timestep <= steps);
    */
}
