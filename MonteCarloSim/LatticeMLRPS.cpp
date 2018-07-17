#include "LatticeMLRPS.hpp"

using namespace std;

LatticeMLRPS::LatticeMLRPS() : Lattice()
{
    RPSMin = 64;
    RPSMax = 192;
    orientation = 1;
    interfaceDistance = 64;
    mobilityRateRPS = 0.1;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            latt[x][y].setRPSDiffusion(mobilityRateRPS);
        }
    }
}

LatticeMLRPS::LatticeMLRPS(string path, int orr, int latticeSize, double mobility, double mobilityRPS, int intDist, int binSize) : Lattice(path, latticeSize, mobility, binSize)
{
    if (interfaceDistance >= latticeSize / 2) 
    {
        char err[] = "InvalidArgumentError: interfaceDistance must be less than or equal to latticeSize/2";
        cout << err << endl;
        throw err;
    }

    interfaceDistance = intDist;
    RPSMin = interfaceDistance;
    RPSMax = latticeSize - 1 - interfaceDistance;
    orientation = orr;
    topology = 0;
    mobilityRateRPS = mobilityRPS;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            latt[x][y].setRPSDiffusion(mobilityRateRPS);
        }
    }
}

LatticeMLRPS::LatticeMLRPS(string path, int orr, int patchTopology, int xSize, int ySize, double mobility, double mobilityRPS, int intDist, int binSize) : Lattice(path, xSize, ySize, mobility, binSize)
{
    orientation = orr;
    topology = patchTopology;
    interfaceDistance = intDist;
    mobilityRateRPS = mobilityRPS;

    if (topology == 1)
    {
        if (interfaceDistance >= sizeY)
        {
            char err[] = "InvalidArgumentError: interfaceDistance must be less than or equal to latticeSize/2";
            cout << err << endl;
            throw err;
        }
        RPSMin = 0;
        RPSMax = interfaceDistance;
        orientation = 0;
    }
    else
    {
        if (interfaceDistance >= sizeX / 2 || interfaceDistance >= sizeY / 2) 
        {
            throw "InvalidArgumentError: interfaceDistance must be less than or equal to half the lattice Size in either Axis";
        }
    }

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            latt[x][y].setRPSDiffusion(mobilityRateRPS);
        }
    }
}

/*(LatticeMLRPS::~LatticeMLRPS()
{
    cout << "~LatticeMLRPS()" << endl;
    for (int i = 0; i < sizeX; ++i) 
    {
        delete[] latt[i];
    }

    delete[] latt;
}


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
    data << "RPS Mobility: " << mobilityRateRPS << endl;
    data << "Interface Distance: " << interfaceDistance << endl;
    data << "Orientation: " << orientation << endl;
    data << "Patch Topology: ";
    switch (topology) {
        case 1:
            data << "Annular" << endl;
            break;
        default:
            data << "Normal" << endl;
            break;
    }
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;

    data.close();
}

void LatticeMLRPS::metadata(int start, int interval, int stop, int swap, int swapInterval)
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
    data << "RPS Mobility: " << mobilityRateRPS << endl;
    data << "Interface Distance: " << interfaceDistance << endl;
    data << "Orientation: " << orientation << endl;
    data << "Patch Topology: ";
    switch (topology) {
        case 1:
            cout << "Annular" << endl;
            break;
        default:
            cout << "Normal" << endl;
            break;
    }
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;
    data << "Swap Time: " << swap << endl;
    data << "Swap Interval: " << swapInterval << endl;

    data.close();
}

void LatticeMLRPS::metadata(int start, int interval, int stop, int startDrive, int driveFrequency, int pulseWidth)
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
    data << "RPS Mobility: " << mobilityRateRPS << endl;
    data << "Interface Distance: " << interfaceDistance << endl;
    data << "Orientation: " << orientation << endl;
    data << "Patch Topology: ";
    switch (topology) {
        case 1:
            data << "Annular" << endl;
            break;
        default:
            data << "Normal" << endl;
            break;
    }
    data << "Start: " << start << endl;
    data << "Interval: " << interval << endl;
    data << "Stop: " << stop << endl;
    data << "Start Drive: " << startDrive << endl;
    data << "Drive Frequency: " << driveFrequency << endl;
    data << "Pulse Width: " << pulseWidth << endl;

    data.close();
}

void LatticeMLRPS::RPSReaction(int x, int y)
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

    double norm = curr.getRPSPredation() + curr.getRPSDiffusion();
    double swapProb = curr.getRPSDiffusion() / norm;

    double rand = actionDist(rng);

    Cell & neighbor = latt[X][Y];

    int currSpec = curr.getSpecies();
    int neighSpec = neighbor.getSpecies();
    if (rand < swapProb && neighSpec != currSpec) //Pair Swapping
    {
        //updateCurrent(curr.getSpecies(), boundary, direction);
        //if (tmp != 3) 
        //{
            //updateCurrent(tmp, boundary, direction * -1);
        //}

        neighbor.setSpecies(currSpec);
        curr.setSpecies(neighSpec);

        if (Y != y)
        {
            updateBinnedReactionCount(2, currSpec, y);
            if (neighSpec != 3)
            {
                updateBinnedReactionCount(2, neighSpec, Y);
            }
        }
        //timestep++;
    }
    else if (rand >= swapProb) //Predation
    {
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(curr.getSpecies());
            updateBinnedReactionCount(0, neighSpec, Y);
            updateBinnedReactionCount(1, neighSpec, Y);
            //updateCurrent(curr.getSpecies(), boundary, direction);
            //timestep++;
        }
    }

}

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord)
{
    int p = sizeX * sizeY;

    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps);

    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep % interval == 0 && timestep == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            //cout << timestep << endl;
            if (monteCarloStep >= startRecord)
            {
                updateFlux();
                updateBinnedFlux();
                dataOutput();
            }

            clearBinnedReactionCount();
        }
        
        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep ++;

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep++;
        }
        while (latt[x][y].getSpecies() > 2);

        //timestep++;
 
        if ( (y >= RPSMin && y <= RPSMax) ) 
        {
            if (topology == 1)
            {
                RPSReaction(x, y);
            }
            else if (x >= RPSMin && x <= RPSMax)
            {
                switch(orientation)
                {
                    case 0  :   RPSReaction(x, y); break;
                    case 1  :   reaction(x, y); break;
                    default :   RPSReaction(x, y); break;
                }
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }
        else
        {
            if (topology == 1)
            {
                reaction(x, y);
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }

        if (timestep >= p) 
        {
            timestep = 0;

            updateDensity();
            updateBinnedDensity();
        
            //clearCurrent();
            monteCarloStep++;
        }

    }
    while (monteCarloStep <= steps);
    cout << endl << "Simulation Complete" << endl;
}

void LatticeMLRPS::monteCarloRun(int steps, int interval, int startRecord, int swapTime, int swapInterval)
{
    int p = sizeX * sizeY;

    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps, swapTime, swapInterval);

    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep < swapTime && monteCarloStep % interval == 0 && timestep == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            //cout << timestep << endl;
            if (monteCarloStep >= startRecord)
            {
                updateFlux();
                updateBinnedFlux();
                dataOutput();
            }
        }
        else if (monteCarloStep >= swapTime && monteCarloStep % swapInterval == 0 && timestep == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            //cout << timestep << endl;
            updateFlux();
            dataOutput();
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
        
        //timestep++;
 
        if ( (y >= RPSMin && y <= RPSMax) && monteCarloStep >= swapTime) 
        {
            if (topology == 1)
            {
                RPSReaction(x, y);
            }
            else if (x >= RPSMin && x <= RPSMax)
            {
                switch(orientation)
                {
                    case 0  :   RPSReaction(x, y); break;
                    case 1  :   reaction(x, y); break;
                    default :   RPSReaction(x, y); break;
                }
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }
        else
        {
            if (topology == 1)
            {
                reaction(x, y);
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }

        if (timestep >= p) 
        {
            timestep = 0;
            updateDensity();
            updateBinnedDensity();

            //clearCurrent();
            monteCarloStep++;
        }

    }
    while (monteCarloStep <= steps);
    cout << endl << "Simulation Complete" << endl;
}

void LatticeMLRPS::drivenMonteCarloRun(int steps, int interval, int startRecord, int startDrive, int driveFrequency, int pulseWidth)
{
    int p = sizeX * sizeY;

    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps, startDrive, driveFrequency, pulseWidth);

    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep % interval == 0 && timestep == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            //cout << timestep << endl;
            if (monteCarloStep >= startRecord)
            {
                updateFlux();
                updateBinnedFlux();
                dataOutput();
            }
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

        //timestep++;
 
        if ( (y >= RPSMin && y <= RPSMax) && (monteCarloStep >= startDrive) && ((monteCarloStep - startDrive) % driveFrequency <= pulseWidth)) 
        {
            if (topology == 1)
            {
                RPSReaction(x, y);
            }
            else if (x >= RPSMin && x <= RPSMax)
            {
                switch(orientation)
                {
                    case 0  :   RPSReaction(x, y); break;
                    case 1  :   reaction(x, y); break;
                    default :   RPSReaction(x, y); break;
                }
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }
        else
        {
            if (topology == 1)
            {
                reaction(x, y);
            }
            else
            {
                switch(orientation)
                {
                    case 0  :   reaction(x, y); break;
                    case 1  :   RPSReaction(x, y); break;
                    default :   reaction(x, y); break;
                }
            }
        }

        if (timestep >= p) 
        {
            timestep = 0;

            updateDensity();
            updateBinnedDensity();

            //clearCurrent();
            monteCarloStep++;
        }

    }
    while (monteCarloStep <= steps);
    cout << endl << "Simulation Complete" << endl;
}
