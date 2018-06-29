#include "Lattice.hpp"

using namespace std;

Lattice::Lattice() : rng(std::time(0)), coordDist(0, 256), neighDist(0, 3)
{
    sizeX = 256;
    sizeY = 256;

    double popDist[] = {0.3, 0.3, 0.3, 0.1};
    filePath = "";

    mobilityRate = 1.25;

    latt = new Cell*[sizeX];

    for (int i = 0; i < sizeX; i++)
    {
        latt[i] = new Cell[sizeY];
    }

    timestep = 0;

    aPop = bPop = cPop = 0;

    boost::random::discrete_distribution<> pop(popDist);

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            incrementSpeciesCount(spec);
            latt[x][y].setSwapRate(mobilityRate);
        }
    }

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

/*
 * The constructor for the lattice. 
 *
 * @param double * popDist      this should be an array describing the probability with which
 *                              any given cell will be initialized to a certain species.
 *                              (e.g. if you want each cell to have a 30% chance of being
 *                              assigned A, B, or C and a 10% chance of being empty, then you
 *                              would pass {0.3, 0.3, 0.3, 0.1})
 *
 * @param string path           the directory where output files will be written.
 */
Lattice::Lattice(string path, int lattSize, double mobility) : rng(std::time(0)), coordDist(0, lattSize - 1), neighDist(0, 3), actionDist()
{
    double popDist[] = {0.25, 0.25, 0.25, 0.25};

    sizeX = lattSize;
    sizeY = lattSize;

    mobilityRate = mobility;

    timestep = 0;
    filePath = path;

    latt = new Cell*[sizeX];

    for (int i = 0; i < sizeY; i++)
    {
        latt[i] = new Cell[sizeY];
    }

    aPop = bPop = cPop = 0;

    boost::random::discrete_distribution<> pop(popDist);

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            incrementSpeciesCount(spec);
            latt[x][y].setSwapRate(mobility);
        }
    }

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

Lattice::~Lattice()
{
    cout << "~Lattice()" << endl;
    for (int i = 0; i < sizeX; ++i) 
    {
        delete[] latt[i];
    }

    delete[] latt;
}

void Lattice::incrementSpeciesCount(int spec)
{
    switch(spec)
        {
            case 0: aPop++; break;
            case 1: bPop++; break;
            case 2: cPop++; break;
        }
}

void Lattice::decrementSpeciesCount(int spec)
{
    switch(spec)
        {
            case 0: aPop--; break;
            case 1: bPop--; break;
            case 2: cPop--; break;
        }
}

void Lattice::metadata(int start, int interval, int stop)
{
    using namespace std;
    fstream data("metadata.txt", ofstream::out | ofstream::app | ofstream::in);

    data << "X Size: " << sizeX << endl;
    data << "Y Size: " << sizeY << endl;
    data << "Mobility: " << mobilityRate << endl;
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;

    data.close();
}

void Lattice::reaction(int x, int y)
{
    int neigh = neighDist(rng);

    int X = x, Y = y;
    
    switch(neigh)
    {
        case 0 : //up
            if (x == 0)
            {
                X = sizeX - 1;
            }
            else
            {
                X = (x - 1) % sizeX;
            }
            break;
        case 1 : //right
            Y = (y + 1) % sizeY;
            break;
        case 2 : //down
            X = (x + 1) % sizeX;
            break;
        case 3 : //left
            if (y == 0)
            {
                Y = sizeY - 1;
            }
            else
            {
                Y = (y - 1) % sizeY;
            }
            break;
    }

    
    Cell & curr = latt[x][y];

    double norm = curr.getPredRate() + curr.getFertRate() + curr.getSwapRate();
    double swapProb = curr.getSwapRate() / norm;
    double predProb = curr.getPredRate() / norm;

    double rand = actionDist(rng);

    Cell & neighbor = latt[X][Y];

    if (rand < swapProb) //Pair Swapping
    {
        int tmp = neighbor.getSpecies();
        neighbor.setSpecies(curr.getSpecies());
        curr.setSpecies(tmp);
    }
    else if (rand >= swapProb && rand < swapProb + predProb) //Predation
    {
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(3);
        }
    }
    else if (rand >= swapProb + predProb) //Reproduction
    {
        if (neighbor.getSpecies() == 3)
        {
            neighbor.setSpecies(curr.getSpecies());
            incrementSpeciesCount(curr.getSpecies());
        }
    }
}

void Lattice::dataOutput()
{
    using namespace std;
    stringstream ss;
    ss << filePath << "MLStep" << "_" << timestep << ".csv";
    //ss << filePath << "S" << size << "_" << timestep << ".ppm";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    //data << "P3\n" << size << " " << size << endl;
    //data << "#" << fileName << "\n" << "1" << endl;

    cout <<  "writing " << fileName.c_str() << endl;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            int spec = latt[x][y].getSpecies();

            data << spec;
            /*switch(spec)
            {
                case 0  :   data << "r" ; break;
                case 1  :   data << "g" ; break;
                case 2  :   data << "b" ; break;
                case 3  :   data << "k" ; break;
                default :   data << "y" ; break;
            }*/

            if (y < sizeY - 1)
            {
                data << ",";
            }
        }

        if (x < sizeX - 1)
        {
            data << endl;
        }
    }

    cout << "Current Population: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
    data.close();
}

void Lattice::reactTest()
{
    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            Cell & cell = latt[x][y];
            cell.setSpecies(0);
            timestep++;
            dataOutput();
        }
    }
    
}

void Lattice::monteCarloRun(int steps, int interval, int start)
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
 
        reaction(x, y);

        if (timestep % interval == 0)
        {
            cout << timestep << endl;
            if (timestep >= start)
            {
                dataOutput();
            }
        }

        timestep++;
    }
    while (timestep <= steps);
}

