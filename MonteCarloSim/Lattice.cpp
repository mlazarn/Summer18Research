#include "Lattice.hpp"

using namespace std;

Lattice::Lattice() : rng(std::time(0)), xCoordDist(0, 256), yCoordDist(0, 256), neighDist(0, 3)
{
    sizeX = 256;
    sizeY = 256;

    double popDist[] = {0.3, 0.3, 0.3, 0.1};
    filePath = "";

    mobilityRate = 1.25;

    latt = new Cell*[sizeX];

    monteCarloStep = 0;
    timestep = 0;

    aPop = bPop = cPop = 0;

    boost::random::discrete_distribution<> pop(popDist);
    
    flux = new double*[3];
    density0 = new double*[3];
    density1 = new double*[3];
    
    for (int i = 0; i < 3; i++)
    {
        //cout << "inititalizing row #" << i << endl;
        flux[i] = new double[sizeY];
        density0[i] = new double[sizeY];
        density1[i] = new double[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            //cout << "inititalizing column #" << y << endl;
            density0[i][y] = 0.0;
            density1[i][y] = 0.0;
            flux[i][y] = 0.0;
        }

        /*
        current[i] = new int[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            current[i][y] = 0;
        }
        */
    }

    for (int x = 0; x < sizeX; x++)
    {
        latt[x] = new Cell[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            latt[x][y].setSwapRate(mobilityRate);
            latt[x][y].setDifRate(mobilityRate);
            incrementSpeciesCount(spec);
        }
    }

    updateDensity();

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
Lattice::Lattice(string path, int lattSize, double mobility) : rng(std::time(0)), xCoordDist(0, lattSize - 1), yCoordDist(0, lattSize - 1), neighDist(0, 3), actionDist()
{
    double popDist[] = {0.25, 0.25, 0.25, 0.25};

    sizeX = lattSize;
    sizeY = lattSize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

    latt = new Cell*[sizeX];

    aPop = bPop = cPop = 0;

    boost::random::discrete_distribution<> pop(popDist);
    
    flux = new double*[3];
    density0 = new double*[3];
    density1 = new double*[3];
    
    for (int i = 0; i < 3; i++)
    {
        //cout << "inititalizing arrays" << endl;
        flux[i] = new double[sizeY];
        density0[i] = new double[sizeY];
        density1[i] = new double[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            density0[i][y] = 0.0;
            density1[i][y] = 0.0;
            flux[i][y] = 0.0;
        }

        /*
        current[i] = new double[sizeY - 1];
        for (int y = 0; y < sizeY - 1; y++)
        {
            current[i][y] = 0;
        }
        */
    }

    for (int x = 0; x < sizeX; x++)
    {
        latt[x] = new Cell[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            latt[x][y].setSwapRate(0.8);
            latt[x][y].setDifRate(0.8);
            incrementSpeciesCount(spec);
            latt[x][y].setSwapRate(mobility);
        }
    }
    
    updateDensity();

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

Lattice::Lattice(string path, int xSize, int ySize, double mobility) : rng(std::time(0)), xCoordDist(0, xSize - 1), yCoordDist(0, ySize - 1), neighDist(0, 3), actionDist()
{
    double popDist[] = {0.25, 0.25, 0.25, 0.25};

    sizeX = xSize;
    sizeY = ySize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

    latt = new Cell*[sizeX];

    aPop = bPop = cPop = 0;

    boost::random::discrete_distribution<> pop(popDist);
    
    flux = new double*[3];
    density0 = new double*[3];
    density1 = new double*[3];

    for (int i = 0; i < 3; i++)
    {
        //cout << "inititalizing arrays" << endl;
        flux[i] = new double[sizeY];
        density0[i] = new double[sizeY];
        density1[i] = new double[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            density0[i][y] = 0.0;
            density1[i][y] = 0.0;
            flux[i][y] = 0.0;
        }

        /*
        current[i] = new int[sizeY - 1];
        for (int y = 0; y < sizeY; y++)
        {
            current[i][y] = 0;
        }
        */
    }

    for (int x = 0; x < sizeX; x++)
    {
        latt[x] = new Cell[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            latt[x][y].setSwapRate(mobilityRate);
            latt[x][y].setDifRate(mobilityRate);
            incrementSpeciesCount(spec);
        }
    }
    
    updateDensity();

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

Lattice::~Lattice()
{
    for (int i = 0; i < 3; i++)
    {
        delete[] flux[i];
        delete[] density0[i];
        delete[] density1[i];
    }
    for (int x = 0; x < sizeX; ++x) 
    {
        delete[] latt[x];
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

void Lattice::updateDensity()
{
    //cout << "updating density" << endl;
    for (int y = 0; y < sizeY; y++) 
    {
        int count[3] = {0, 0, 0};
        for (int x = 0; x < sizeX; x++) 
        {
            int spec = latt[x][y].getSpecies();
            if (spec < 3)
            {
                count[spec]++;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            density0[i][y] = density1[i][y];
            density1[i][y] = (1.0 * count[i]) / sizeX;
        }
    }
    //cout << "density updated" << endl;
}

void Lattice::updateFlux()
{
    //cout << "updating flux" << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            flux[i][y] = density1[i][y] - density0[i][y];
        }
    }
    //cout << "flux updated" << endl;
}

/*
void Lattice::updateCurrent(int spec, int y, int direction)
{
    current[spec][y] += direction;
}


void Lattice::clearCurrent()
{
    for (int i = 0; i < 3; i++) 
    {
        for (int y = 0; y < sizeY - 1; y++) 
        {
            current[i][y] = 0;
        }
    }
}
*/

void Lattice::metadata(int start, int interval, int stop)
{
    using namespace std;
    stringstream ss;
    ss << filePath << "metadata.txt";
    //ss << filePath << "S" << size << "_" << timestep << ".ppm";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    data << "X Size: " << sizeX << endl;
    data << "Y Size: " << sizeY << endl;
    data << "Mobility: " << mobilityRate << endl;
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;

    data.close();
}

void Lattice::progressBar(float progress)
{
    if (progress >= 0.0 && progress <= 1.0)
    {
        int barWidth = 70;

        cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; i++)
        {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";

        }
        cout << "] " << int(progress * 100.0) << " %\r";
        cout.flush();
    }
}

void Lattice::reaction(int x, int y)
{
    int neigh = neighDist(rng);

    int X = x, Y = y;

    //int direction = 0;
    //int boundary = y;
    
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
            //boundary = Y;
            //direction = 1;
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
            //direction = -1;
            break;
    }

    
    Cell & curr = latt[x][y];

    double norm = curr.getPredRate() + curr.getFertRate() + curr.getSwapRate();
    double swapProb = curr.getSwapRate() / norm;
    double predProb = curr.getPredRate() / norm;

    double rand = actionDist(rng);

    Cell & neighbor = latt[X][Y];

    if (rand < swapProb && neighbor.getSpecies() != curr.getSpecies()) //Pair Swapping
    {
        int tmp = neighbor.getSpecies();

        //updateCurrent(curr.getSpecies(), boundary, direction);
        //if (tmp != 3) 
        //{
            //updateCurrent(tmp, boundary, direction * -1);
        //}

        neighbor.setSpecies(curr.getSpecies());
        curr.setSpecies(tmp);
        //timestep++;
    }
    else if (rand >= swapProb && rand < swapProb + predProb) //Predation
    {
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(3);
            //timestep++;
        }
    }
    else if (rand >= swapProb + predProb) //Reproduction
    {
        if (neighbor.getSpecies() == 3)
        {
            neighbor.setSpecies(curr.getSpecies());
            incrementSpeciesCount(curr.getSpecies());
            //updateCurrent(curr.getSpecies(), boundary, direction);
            //timestep++;
        }
    }
}

void Lattice::dataOutput()
{
    using namespace std;
    stringstream ss;
    ss << filePath << "latt" << "_" << monteCarloStep << ".csv";
    //ss << filePath << "S" << size << "_" << timestep << ".ppm";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    //data << "P3\n" << size << " " << size << endl;
    //data << "#" << fileName << "\n" << "1" << endl;

    //cout <<  "writing " << fileName.c_str() << endl;

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

    data.close();

    ss.str("");
    ss << filePath << "density" << "_" << monteCarloStep << ".csv";
    fileName = ss.str();

    fstream dataDensity(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    ss.str("");
    ss << filePath << "flux" << "_" << monteCarloStep << ".csv";
    fileName = ss.str();

    fstream dataFlux(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            double dns = density1[i][y];
            double flx = flux[i][y];

            dataDensity << dns;
            dataFlux << flx;

            if (y < sizeY - 1)
            {
                dataDensity << ",";
                dataFlux << ",";
            }
        }

        if (i < 2)
        {
            dataDensity << endl;
            dataFlux << endl;
        }
    }

    dataDensity.close();
    dataFlux.close();
}

void Lattice::monteCarloRun(int steps, int interval, int start)
{
    int p = sizeX * sizeY;
    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        int x = xCoordDist(rng);
        int y = yCoordDist(rng);

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
        }
        while (latt[x][y].getSpecies() > 2);
 
        reaction(x, y);
        timestep++;

        if (timestep >= p)
        {
            timestep = 0;
            monteCarloStep++;
        }

        if (monteCarloStep % interval == 0)
        {
            //cout << timestep << endl;
            if (monteCarloStep >= start)
            {
                dataOutput();
            }
        }

    }
    while ( monteCarloStep <= steps);
}

