#include "Lattice.hpp"

using namespace std;

// default constructor
Lattice::Lattice() : rng(std::time(0)), xCoordDist(0, 256), yCoordDist(0, 256), neighDist(0, 3)
{
    sizeX = 256;
    sizeY = 256;

    binWidth = 4;

    filePath = "";

    mobilityRate = 1.25;

    monteCarloStep = 0;
    timestep = 0;

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();

    updateDensity();
    updateBinnedDensity();
}

/*
 * The square constructor for the lattice. 
 *
 * @param path       string      the directory where output files will be 
 *                               written
 * @param lattSize   int         the number of rows and columns in the 
 *                               lattice
 * @param mobility   double      the mobility rate of the cells in the 
 *                               lattice. Use values greater than or equal 
 *                               to 0.
 * @param binSize    int         the width of bins for data reduction.
 */
Lattice::Lattice(string path, int lattSize, double mobility, int binSize) : rng(std::time(0)), xCoordDist(0, lattSize - 1), yCoordDist(0, lattSize - 1), neighDist(0, 3), actionDist()
{
    sizeX = lattSize;
    sizeY = lattSize;

    binWidth = binSize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();

    updateDensity();
    updateBinnedDensity();
}

/*
 * The rectangular constructor for the lattice.
 *
 * @param path      string  the directory where output files will be written
 * @param xSize     int     the number of rows in the lattice
 * @param ySize     int     the number of rows in the lattice
 * @param mobility  double  the mobility rate of the cells in the lattice. Use 
 *                          values greater than or equal to 0.
 * @param binSize   int     the width of bins for data reduction.
 */
Lattice::Lattice(string path, int xSize, int ySize, double mobility, int binSize) : rng(std::time(0)), xCoordDist(0, xSize - 1), yCoordDist(0, ySize - 1), neighDist(0, 3), actionDist()
{
    sizeX = xSize;
    sizeY = ySize;

    binWidth = binSize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();
    
    updateDensity();
    updateBinnedDensity();
}

//Destructor.
Lattice::~Lattice()
{
    for (int i = 0; i < 3; i++)
    {
        delete[] binnedDeathCounts[i];
        delete[] binnedBirthCounts[i];
        delete[] binnedDiffusionCounts[i];
        delete[] binnedFlux[i];
        delete[] binnedDensity0[i];
        delete[] binnedDensity1[i];
        delete[] flux[i];
        delete[] density0[i];
        delete[] density1[i];
    }
    for (int x = 0; x < sizeX; ++x) 
    {
        delete[] latt[x];
    }

    delete binnedDeathCounts;
    delete binnedBirthCounts;
    delete binnedDiffusionCounts;
    delete binnedFlux;
    delete binnedDensity0;
    delete binnedDensity1;
    delete flux;
    delete density0;
    delete density1;

    delete latt;
}

void Lattice::initializeArrays()
{
    binnedArraySize = sizeY / binWidth;
    binMidpoints = new double[binnedArraySize];
    binnedDeathCounts = new int*[3];
    binnedBirthCounts = new int*[3];
    binnedDiffusionCounts = new int*[3];
    binnedFlux = new double*[3];
    binnedDensity0 = new double*[3];
    binnedDensity1 = new double*[3];
    
    flux = new double*[3];
    density0 = new double*[3];
    density1 = new double*[3];
  
    for (int i = 0; i < 3; i++)
    {
        //cout << "inititalizing row #" << i << endl;
        flux[i] = new double[sizeY];
        density0[i] = new double[sizeY];
        density1[i] = new double[sizeY];
    
        binnedDeathCounts[i] = new int[binnedArraySize];
        binnedBirthCounts[i] = new int[binnedArraySize];
        binnedDiffusionCounts[i] = new int[binnedArraySize];
        binnedFlux[i] = new double[binnedArraySize];
        binnedDensity0[i] = new double[binnedArraySize];
        binnedDensity1[i] = new double[binnedArraySize];
        for (int y = 0; y < sizeY; y++)
        {
            density0[i][y] = 0.0;
            density1[i][y] = 0.0;
            flux[i][y] = 0.0;
    
            int binY = y / binWidth;
            if (i == 0)
            {
                binMidpoints[binY] = (((2.0 * binY) + 1.0) * (binWidth)) / 2.0;
            }
            binnedDeathCounts[i][binY] = 0;
            binnedBirthCounts[i][binY] = 0;
            binnedDiffusionCounts[i][binY] = 0;
            binnedFlux[i][binY] = 0.0;
            binnedDensity0[i][binY] = 0.0;
            binnedDensity1[i][binY] = 0.0;
        }
    }
}

void Lattice::initializeLattice()
{
    double popDist[] = {0.3, 0.3, 0.3, 0.1};
    boost::random::discrete_distribution<> pop(popDist);

    latt = new Cell*[sizeX];
    specLat = new int*[sizeX];

    for (int x = 0; x < sizeX; x++)
    {
        latt[x] = new Cell[sizeY];
        specLat[x] = new int[sizeY];
        for (int y = 0; y < sizeY; y++)
        {
            int spec;
            spec = pop(rng);
            latt[x][y].setSpecies(spec);
            specLat[x][y] = spec;
            latt[x][y].setSwapRate(mobilityRate);
            latt[x][y].setDifRate(mobilityRate);
            incrementSpeciesCount(spec);
        }
    }
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

double Lattice::globalDensity()
{
    int sum = aPop;
    return ((1.0 * sum) / (sizeX * sizeY)) ;
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

void Lattice::updateBinnedDensity()
{
    //double binSum[3][binnedArraySize];

    for (int i = 0; i < 3; i++)
    {
        for (int binY = 0; binY < binnedArraySize; binY++)
        {
            double cumsum = 0.0;
            //binSum[i][binY] = 0.0;

            for (int n = 0; n < binWidth; n++)
            {
                int y = (binY * binWidth) + n;
                cumsum += density1[i][y];
            }
            binnedDensity0[i][binY] = binnedDensity1[i][binY];
            binnedDensity1[i][binY] = cumsum / binWidth;
        }
    }

    //for (int i = 0; i < 3; i++)
    //{
        //for (int y = 0; y < sizeY; y++)
        //{
            //int binY = y / binWidth;
            //double tmp = binSum[i][binY] + density1[i][y];
            //binSum[i][binY] = tmp;

        //}
    //}

    //for (int i = 0; i < 3; i++)
    //{
        //for (int binY = 0; binY < binnedArraySize; binY++)
        //{
            //binnedDensity0[i][binY] = binnedDensity1[i][binY];
            //binnedDensity1[i][binY] = binSum[i][binY] / binWidth;
        //}
    //}
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

void Lattice::updateBinnedFlux()
{
    //double binSum[3][binnedArraySize];

    //for (int i = 0; i < 3; i++)
    //{
        //for (int binY = 0; binY < binnedArraySize; binY++)
        //{
            //binSum[i][binY] = 0.0;
        //}
    //}

    //for (int i = 0; i < 3; i++)
    //{
        //for (int y = 0; y < sizeY; y++)
        //{
            //int binY = y / binWidth;
            //method 1: binning the raw flux
            //binSum[i][binY] += flux[i][y];

        //}
    //}

    for (int i = 0; i < 3; i ++)
    {
        for (int binY = 0; binY < binnedArraySize; binY++)
        {
            //method 1: binning the raw flux
            //binnedFluxA[i][binY] = binSum[i][binY] / binWidth;
            //method 2: taking the difference of the binned densities
            binnedFlux[i][binY] = binnedDensity1[i][binY] - binnedDensity0[i][binY];
        }
    }
}

void Lattice::clearBinnedReactionCount()
{
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < binnedArraySize; y++)
        {
            binnedDeathCounts[i][y] = 0;
            binnedBirthCounts[i][y] = 0;
            binnedDiffusionCounts[i][y] = 0;
        }
    }
}

void Lattice::updateBinnedReactionCount(int reaction, int species, int y)
{
    int binY = y / binWidth;

    switch (reaction) 
    {
        case 0:
            binnedDeathCounts[species][binY]++;
            break;
        case 1:
            binnedBirthCounts[species][binY]++;
            break;
        case 2:
            binnedDiffusionCounts[species][binY]++;
            break;
    }
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
    data << "Bin Width: " << binWidth << endl;
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
        cout << "] " << int(progress * 100.0) << " %" << " t=" << monteCarloStep << "\r";
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

    int neighSpec = neighbor.getSpecies();
    int currSpec = curr.getSpecies();

    if (rand < swapProb && neighSpec != currSpec) //Pair Swapping
    {
        neighbor.setSpecies(currSpec);
        curr.setSpecies(neighSpec);

        specLat[x][y] = neighSpec; // set current species on the int array
        specLat[X][Y] = currSpec; //set neighbor species on the int array

        if (Y != y)
        {
            updateBinnedReactionCount(2, currSpec, y);
            if (neighSpec != 3)
            {
                updateBinnedReactionCount(2, currSpec, Y);
            }
        }
        //timestep++;
    }
    else if (rand >= swapProb && rand < swapProb + predProb) //Predation
    {
        //if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        if (neighSpec == (currSpec + 1) % 3)
        {
            //decrementSpeciesCount(neighbor.getSpecies());
            decrementSpeciesCount(neighSpec);
            neighbor.setSpecies(3);
            specLat[X][Y] = 3; //set neighbor species to dead on int array
            updateBinnedReactionCount(0, neighSpec, Y);
            //timestep++;
        }
    }
    else if (rand >= swapProb + predProb) //Reproduction
    {
        //if (neighbor.getSpecies() == 3)
        if (neighSpec == 3)
        {
            //neighbor.setSpecies(curr.getSpecies());
            neighbor.setSpecies(currSpec);
            specLat[X][Y] = currSpec;
            incrementSpeciesCount(currSpec);
            updateBinnedReactionCount(1, currSpec, Y);
            //updateCurrent(curr.getSpecies(), boundary, direction);
            //timestep++;
        }
    }
}

void Lattice::binMidpointsOutput()
{
    using namespace std;
    stringstream ss;
    ss << filePath << "bin_midpoints.csv";
    //ss << filePath << "S" << size << "_" << timestep << ".ppm";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);
    
    for (int y = 0; y < binnedArraySize; y++)
    {
        data << binMidpoints[y];
        if (y < binnedArraySize - 1)
        {
            data << ",";
        }
    }

    data.close();
}

void Lattice::dataOutput(int outputType)
{
    using namespace std;
    stringstream ss;

    const char *prefixes[] = {"latt_", "density_", "flux_", "binned_density_", "binned_flux_",
                         "binned_death_counts_", "binned_birth_counts_", "binned_diffusion_counts_"};

    ss << filePath << prefixes[outputType] << monteCarloStep << ".csv" ;

    string fileName;
    fileName = ss.str();

    ofstream data(fileName.c_str(), ofstream::out | ofstream::app );

    ss.str("");
    ss.clear();

    if (outputType == 0)
    {
        for (int x = 0; x < sizeX; x++)
        {
            ss.str("");
            ss.clear();
            for (int y = 0; y < sizeY; y++)
            {
                int spec = latt[x][y].getSpecies();

                ss << spec;

                if (y < sizeY - 1)
                {
                    ss << ",";
                }
            }

            data << ss.str();
            if (x < sizeX - 1)
            {
                data << endl;
            }
        }
    }
    else if (outputType == 1 || outputType == 2)
    {
        for (int i = 0; i < 3; i++)
        {
            ss.str("");
            ss.clear();
            for (int y = 0; y < sizeY; y++)
            {
                switch (outputType) 
                {
                    case 1:
                        ss << density1[i][y];
                        break;
                    case 2:
                        ss << flux[i][y];
                        break;
                }

                if (y < sizeY - 1)
                {
                    ss << ",";
                }
            }
            data << ss.str();
            data << endl;
        }
    }
    else if (outputType > 2)
    {
        for (int i = 0; i < 3; i++)
        {
            ss.str("");
            ss.clear();
            for (int binY = 0; binY < binnedArraySize; binY++)
            {
                switch (outputType) 
                {
                        case 3:
                            ss << binnedDensity1[i][binY];
                            break;
                        case 4:
                            ss << binnedFlux[i][binY];
                            break;
                        case 5:
                            ss << binnedDeathCounts[i][binY];
                            break;
                        case 6:
                            ss << binnedBirthCounts[i][binY];
                            break;
                        case 7:
                            ss << binnedDiffusionCounts[i][binY];
                            break;
                }

                if (binY < binnedArraySize - 1)
                {
                    ss << ",";
                }
            }

            data << ss.str();
            data << endl;
        }
    }

    data.close();
}

void Lattice::monteCarloRun(int steps, int interval, int start)
{
    int p = sizeX * sizeY;
    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep % interval == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            //cout << timestep << endl;
            if (monteCarloStep >= start)
            {
                updateFlux();
                updateBinnedFlux();
                for (int i = 0; i < 9; i++)
                {
                    dataOutput(i);
                }
            }

            clearBinnedReactionCount();
        }

        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep += (1.0 / p);

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep += (1.0 / p);
        }
        while (latt[x][y].getSpecies() > 2);
 
        reaction(x, y);

        if (timestep >= 1)
        {
            timestep = 0.0;
            monteCarloStep++;
        }

    }
    while ( monteCarloStep <= steps);
}

void Lattice::monteCarloDensityRun(int steps, int interval, int start)
{
    int p = sizeX * sizeY;
    double tau = mobilityRate + 2.0;

    stringstream ss;
    ss << filePath << "densities.csv";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep % interval == 0 && timestep == 0.0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            if (monteCarloStep >= start)
            {
                data << globalDensity() << ",";
            }
        }

        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep += (1 / (p * tau));

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep += (1 / (p * tau));
        }
        while (latt[x][y].getSpecies() > 2);
 
        reaction(x, y);

        if (timestep >= 1.0)
        {
            timestep = 0.0;
            monteCarloStep++;
        }

    }
    while ( monteCarloStep <= steps);

    data.close();
}

double Lattice::densityRun(int steps)
{
    int p = sizeX * sizeY;
    double tau = mobilityRate + 2.0;

    do
    {
        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep += (1 / (p * tau));

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep += (1 / (p * tau));
        }
        while (latt[x][y].getSpecies() > 2);
 
        reaction(x, y);

        if (timestep >= 1.0)
        {
            timestep = 0.0;
            monteCarloStep++;
        }

    }
    while (monteCarloStep < steps);

    return globalDensity();
}
