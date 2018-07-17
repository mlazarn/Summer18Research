#include "Lattice.hpp"

using namespace std;

Lattice::Lattice() : rng(std::time(0)), xCoordDist(0, 256), yCoordDist(0, 256), neighDist(0, 3)
{
    sizeX = 256;
    sizeY = 256;

    binWidth = 4;

//    double popDist[] = {0.3, 0.3, 0.3, 0.1};
    filePath = "";

    mobilityRate = 1.25;

//    latt = new Cell*[sizeX];

    monteCarloStep = 0;
    timestep = 0;

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();


    binMidpointsOutput();
    updateDensity();
    updateBinnedDensity();

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
Lattice::Lattice(string path, int lattSize, double mobility, int binSize) : rng(std::time(0)), xCoordDist(0, lattSize - 1), yCoordDist(0, lattSize - 1), neighDist(0, 3), actionDist()
{
//    double popDist[] = {0.3, 0.3, 0.3, 0.1};

    sizeX = lattSize;
    sizeY = lattSize;

    binWidth = binSize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

//    latt = new Cell*[sizeX];

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();

    
    binMidpointsOutput();
    updateDensity();
    updateBinnedDensity();

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

Lattice::Lattice(string path, int xSize, int ySize, double mobility, int binSize) : rng(std::time(0)), xCoordDist(0, xSize - 1), yCoordDist(0, ySize - 1), neighDist(0, 3), actionDist()
{
//    double popDist[] = {0.3, 0.3, 0.3, 0.1};

    sizeX = xSize;
    sizeY = ySize;

    binWidth = binSize;

    mobilityRate = mobility;

    timestep = 0;
    monteCarloStep = 0;

    filePath = path;

//    latt = new Cell*[sizeX];

    aPop = bPop = cPop = 0;

    initializeArrays();
    initializeLattice();
    
    binMidpointsOutput();
    updateDensity();
    updateBinnedDensity();

    cout << "Lattice Initialized" << endl;
    cout << "Initial Populatiom: " << "A: " << aPop << " B:" << bPop << " C: " << cPop << endl;
}

Lattice::~Lattice()
{
    for (int i = 0; i < 3; i++)
    {
        delete[] binnedDeathCounts[i];
        delete[] binnedBirthCounts[i];
        delete[] binnedDiffusionCounts[i];
        delete[] binnedFluxA[i];
        delete[] binnedFluxB[i];
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
    delete binnedFluxA;
    delete binnedFluxB;
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
    binnedFluxA = new double*[3];
    binnedFluxB = new double*[3];
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
        binnedFluxA[i] = new double[binnedArraySize];
        binnedFluxB[i] = new double[binnedArraySize];
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
            binnedFluxA[i][binY] = 0.0;
            binnedFluxB[i][binY] = 0.0;
            binnedDensity0[i][binY] = 0.0;
            binnedDensity1[i][binY] = 0.0;
        }
    }
    cout << "binned Array Size: " << binnedArraySize << endl;
}

void Lattice::initializeLattice()
{
    double popDist[] = {0.3, 0.3, 0.3, 0.1};
    boost::random::discrete_distribution<> pop(popDist);

    latt = new Cell*[sizeX];

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
            binnedFluxB[i][binY] = binnedDensity1[i][binY] - binnedDensity0[i][binY];
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

    int neighSpec = neighbor.getSpecies();
    int currSpec = curr.getSpecies();

    if (rand < swapProb && neighSpec != currSpec) //Pair Swapping
    {
        neighbor.setSpecies(currSpec);
        curr.setSpecies(neighSpec);

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
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(3);
            updateBinnedReactionCount(0, neighSpec, Y);
            //timestep++;
        }
    }
    else if (rand >= swapProb + predProb) //Reproduction
    {
        if (neighbor.getSpecies() == 3)
        {
            neighbor.setSpecies(curr.getSpecies());
            incrementSpeciesCount(curr.getSpecies());
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

        data << endl;
    }

    data.close();

    const char *prefixes[] = {"density_", "flux_", "binned_density_", "binned_flux_a_", "binned_flux_b_",
                         "binned_death_counts_", "binned_birth_counts_", "binned_diffusion_counts_"};

    for (int pfx = 0; pfx < 8; pfx++)
    {
        ss.str("");
        ss << filePath << prefixes[pfx] << monteCarloStep << ".csv";
        fileName = ss.str();

        fstream data(fileName.c_str(), ofstream::out | ofstream :: app | ofstream::in);
        for (int i = 0; i < 3; i++)
        {
            if (pfx < 2)
            {
                for (int y = 0; y < sizeY; y++)
                {
                    switch (pfx) 
                    {
                        case 0:
                            data << density1[i][y];
                            break;
                        case 1:
                            data << flux[i][y];
                            break;
                    }

                    if (y < sizeY - 1)
                    {
                        data << ",";
                    }
                }
                data << endl;
            }
            else
            {
                for (int binY = 0; binY < binnedArraySize; binY++)
                {
                    switch (pfx) 
                    {
                        case 2:
                            data << binnedDensity1[i][binY];
                            break;
                        case 3:
                            data << binnedFluxA[i][binY];
                            break;
                        case 4:
                            data << binnedFluxB[i][binY];
                            break;
                        case 5:
                            data << binnedDeathCounts[i][binY];
                            break;
                        case 6:
                            data << binnedBirthCounts[i][binY];
                            break;
                        case 7:
                            data << binnedDiffusionCounts[i][binY];
                            break;
                    }

                    if (binY < binnedArraySize - 1)
                    {
                        data << ",";
                    }
                }
                data << endl;
            }
        }
        data.close();

    }
//    ss.str("");
//    ss << filePath << "density" << "_" << monteCarloStep << ".csv";
//    fileName = ss.str();
//
//    fstream dataDensity(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);
//
//    ss.str("");
//    ss << filePath << "flux" << "_" << monteCarloStep << ".csv";
//    fileName = ss.str();
//
//    fstream dataFlux(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);
//
//    for (int i = 0; i < 3; i++)
//    {
//        for (int y = 0; y < sizeY; y++)
//        {
//            double dns = density1[i][y];
//            double flx = flux[i][y];
//
//            dataDensity << dns;
//            dataFlux << flx;
//
//            if (y < sizeY - 1)
//            {
//                dataDensity << ",";
//                dataFlux << ",";
//            }
//        }
//
//        dataDensity << endl;
//        dataFlux << endl;
//    }
//
//    dataDensity.close();
//    dataFlux.close();
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
                dataOutput();
            }

            clearBinnedReactionCount();
        }

        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep++;

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep++;
        }
        while (latt[x][y].getSpecies() > 2);
 
        reaction(x, y);

        if (timestep >= p)
        {
            timestep = 0;
            monteCarloStep++;
        }

    }
    while ( monteCarloStep <= steps);
}

