#include "LatticeMLRPS.hpp"

using namespace std;

LatticeMLRPS::LatticeMLRPS() : Lattice()
{
    RPSMin = 64;
    RPSMax = 192;
    orientation = 1;
    interfaceDistance = 64;
}

LatticeMLRPS::LatticeMLRPS(string path, int orientation, int latticeSize, double mobility, int interfaceDistance) : Lattice(path, latticeSize, mobility)
{
    if (interfaceDistance >= latticeSize / 2) 
    {
        throw "InvalidArgumentError: interfaceDistance must be less than or equal to latticeSize/2";
    }

    RPSMin = interfaceDistance;
    RPSMax = latticeSize - 1 - interfaceDistance;
    orientation = orientation;
    interfaceDistance = interfaceDistance;
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

void LatticeMLRPS::metadata(int start, int interval, int stop)
{
    using namespace std;
    fstream data("metadata.txt", ofstream::out | ofstream::app | ofstream::in);

    data << "Size: " << size << endl;
    data << "Mobility: " << mobilityRate << endl;
    data << "Interface Distance: " << interfaceDistance << endl;
    data << "Orientation: " << orientation << endl;
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;

    data.close();
}

void LatticeMLRPS::metadata(int start, int interval, int stop, int swap, int swapInterval)
{
    using namespace std;
    fstream data("metadata.txt", ofstream::out | ofstream::app | ofstream::in);

    data << "Size: " << size << endl;
    data << "Mobility: " << mobilityRate << endl;
    data << "Interface Distance: " << interfaceDistance << endl;
    data << "Orientation: " << orientation << endl;
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;
    data << "Swap Time: " << swap << endl;
    data << "Swap Interval: " << swapInterval << endl;

    data.close();
}

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

    double norm = curr.getPredRate() + curr.getSwapRate();
    double swapProb = curr.getSwapRate() / norm;

    double rand = actionDist(rng);

    Cell & neighbor = latt[X][Y];

    if (rand < swapProb) //Pair Swapping
    {
        int tmp = neighbor.getSpecies();
        neighbor.setSpecies(curr.getSpecies());
        curr.setSpecies(tmp);
    }
    else if (rand >= swapProb) //Predation
    {
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(curr.getSpecies());
        }
    }

    /*int neigh = neighDist(rng);

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
        if (rand <= curr.getDifRate() / 2)
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
    */ 
}

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord)
{
    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps);

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
            switch(orientation)
            {
                case 1  :   RPSReaction(x, y); break;
                case 0  :   reaction(x, y); break;
                default :   RPSReaction(x, y); break;
            }
        }
        else
        {
            switch(orientation)
            {
                case 1  :   reaction(x, y); break;
                case 0  :   RPSReaction(x, y); break;
                default :   reaction(x, y); break;
            }
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

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord, int swapTime, int swapInterval)
{
    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps, swapTime, swapInterval);

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
            switch(orientation)
            {
                case 1  :   RPSReaction(x, y); break;
                case 0  :   reaction(x, y); break;
                default :   RPSReaction(x, y); break;
            }
        }
        else
        {
            switch(orientation)
            {
                case 1  :   reaction(x, y); break;
                case 0  :   RPSReaction(x, y); break;
                default :   reaction(x, y); break;
            }
        }

        if (timestep < swapTime && timestep % interval == 0)
        {
            cout << timestep << endl;
            if (timestep >= startRecord)
            {
                dataOutput();
            }
        }
        else if (timestep >= swapTime && timestep % swapInterval == 0)
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

            if (timestep % swapInterval == 0)
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
