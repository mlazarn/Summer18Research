#include "LatticeMLRPS.hpp"

using namespace std;

// Default LatticeMLRPS Constructor. Calls the default Lattice constructor and
// assigns a few values unique to LatticeMLRPS
LatticeMLRPS::LatticeMLRPS() : Lattice()
{
    RPSMin = 64;
    RPSMax = 192;
    orientation = 1;
    interfaceDistance = 64;
    mobilityRateRPS = 0.1;
    recipSizeX = 1.0 / (1.0 * sizeX);

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            latt[x][y].setRPSDiffusion(mobilityRateRPS);
        }
    }
}

// Calls the square Lattice constructor and assigns a few values unique to LatticeMLRPS.
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
    recipSizeX = 1.0 / (1.0 * sizeX);

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            latt[x][y].setRPSDiffusion(mobilityRateRPS);
        }
    }
}

// Calls the rectangular Lattice constructor and assigns a few values unique to LatticeMLRPS.
LatticeMLRPS::LatticeMLRPS(string path, int orr, int patchTopology, int xSize, int ySize, double mobility, double mobilityRPS, int intDist, int binSize) : Lattice(path, xSize, ySize, mobility, binSize)
{
    orientation = orr;
    topology = patchTopology;
    interfaceDistance = intDist;
    mobilityRateRPS = mobilityRPS;
    recipSizeX = 1.0 / (1.0 * sizeX);

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

void LatticeMLRPS::metadata(int start, int interval, int stop)
{
    using namespace std;
    stringstream ss;
    ss << filePath << "metadata.txt";
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

double LatticeMLRPS::autoCorrelator(int spec, int y, int r, int mode)
{
    double sum = 0.0;
    for (int i = 0; i < sizeX; ++i) 
    {
        int targ = i + r;

        if (targ < 0)
        {
            targ = sizeX - targ;
        }
        
        if (specLat[i][y] == spec && specLat[targ % sizeX][y] == spec)
        {
            sum += 1.0;
        }

        //if (symmetry == 1)
        //{
            //int targ = i - r; 
            

            //if (latt[i][y].getSpecies() == spec && latt[(targ) % sizeX][y].getSpecies() == spec)
            //{
                //sum += 1.0;
            //}

            //sum = sum / 2.0;
        //}
    }

    double output;

    if ( mode == 1 )
    {
        output = (sum * recipSizeX) - (density1[spec][y]*density1[spec][y]);
    }
    else 
    {
        output = (sum * recipSizeX);
    }

    return output;
}

//void LatticeMLRPS::outputAutoCorr(int spec, int rMin, int rMax, int subMode)
//{
    //double** autoCorr = new double*[sizeY];
    //for (int i = 0; i < sizeY; i++)
    //{
        //autoCorr[i] = new double[halfX];
    //}

    //using namespace std;
    //stringstream ss;

    //const char *prefixes[] = {"autoCorrA_", "autoCorrB_", "autoCorrC_", "autoCorrAvg_"};

    //int rSize = rMax - rMin;
    //vector<double> corr_function (rSize * sizeY);


    //ofstream data(fileName.c_str(), ofstream::out | ofstream::trunc);

    //int halfX = sizeX/2;
    //ss.precision(6);


    //int rTarg;
    //int targ;
    //double corr;
    //for (int y = 0; y < sizeY; y++)
    //{
        //ss.str("");
        //ss.clear();
        
        //for (int r = rMin; r < rMax; r++)
        //{
            //corr = 0.0;
            //if (spec > 3)
            //{
                //corr = autoCorrelator(0, y, r, subMode);
            //}
            //else if (spec == 3)
            //{
                //corr = autoCorrelator(0, y, r, subMode) 
                     //+ autoCorrelator(1, y, r, subMode)
                     //+ autoCorrelator(2, y, r, subMode);
                //corr = corr / 3.0;
            //}
            //else
            //{
                //corr = autoCorrelator(spec, y, r, subMode);
            //}

            
            //rTarg = r - rMin;

            //targ = (y * rSize) + rTarg;
            //corr_function[targ] = corr;



            //ss << corr; 

            //if (r < halfX - 1)
            //{
                //ss << ",";
            //}
        //}

        //data << ss.str();
        //if (y < sizeY - 1)
        //{
            //data << endl;
            //ss << "\r";
            //ss << endl;
        //}

    //}
    
    //if (spec > 3)
    //{
        //ss << filePath << prefixes[0] << monteCarloStep << ".npy" ;
    //}
    //else 
    //{
        //ss << filePath << prefixes[spec] << monteCarloStep << ".npy" ;
    //}
        
    //string fileName;
    //fileName = ss.str();

    //cnpy::npy_save(fileName, &corr_function[0], {sizeY, rSize}, "w");
    
    //data.close();
//}

// Implements the RPS model reaction.
void LatticeMLRPS::RPSReaction(int x, int y)
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

    double norm = curr.getRPSPredation() + curr.getRPSDiffusion();
    double swapProb = curr.getRPSDiffusion() / norm;

    double rand = actionDist(rng);

    Cell & neighbor = latt[X][Y];

    int currSpec = curr.getSpecies();
    int neighSpec = neighbor.getSpecies();
    if (rand < swapProb && neighSpec != currSpec) //Pair Swapping
    {
        neighbor.setSpecies(currSpec);
        specLat[X][Y] = currSpec; //set neighbor species to currSpec on int array;
        curr.setSpecies(neighSpec);
        specLat[x][y] = neighSpec; //set current species to neighSpec on int array;

        if (Y != y)
        {
            updateBinnedReactionCount(2, currSpec, y);
            if (neighSpec != 3)
            {
                updateBinnedReactionCount(2, neighSpec, Y);
            }
        }
    }
    else if (rand >= swapProb) //Predation
    {
        //if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        if (neighSpec == (currSpec + 1) % 3)
        {
            //decrementSpeciesCount(neighbor.getSpecies());
            decrementSpeciesCount(neighSpec);
            //neighbor.setSpecies(curr.getSpecies());
            neighbor.setSpecies(currSpec);
            specLat[X][Y] = currSpec;
            updateBinnedReactionCount(0, neighSpec, Y);
            updateBinnedReactionCount(1, neighSpec, Y);
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

            if (monteCarloStep >= startRecord)
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
        timestep ++;

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep++;
        }
        while (latt[x][y].getSpecies() > 2);
 
        // Decides whether to use reaction(x, y) or reactionRPS(x, y)
        if ( (y >= RPSMin && y < RPSMax) ) 
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

            if (monteCarloStep >= startRecord)
            {
                updateFlux();
                updateBinnedFlux();
                for (int i = 0; i < 9; i++)
                {
                    dataOutput(i);
                }
            }
        }
        else if (monteCarloStep >= swapTime && monteCarloStep % swapInterval == 0 && timestep == 0)
        {
            float progress = (1.0 * monteCarloStep) / steps;
            progressBar(progress);

            updateFlux();
            for (int i = 0; i < 9; i++)
            {
                dataOutput(i);
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
        
        if ( (y >= RPSMin && y < RPSMax) && monteCarloStep >= swapTime) 
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

            if (monteCarloStep >= startRecord)
            {
                updateFlux();
                updateBinnedFlux();
                for (int i = 0; i < 9; i++)
                {
                    dataOutput(i);
                }
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

        if ( (y >= RPSMin && y < RPSMax) && (monteCarloStep >= startDrive) && ((monteCarloStep - startDrive) % driveFrequency <= pulseWidth)) 
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

            monteCarloStep++;
        }
    }
    while (monteCarloStep <= steps);
    cout << endl << "Simulation Complete" << endl; 
}

void LatticeMLRPS::specAnalysisRun(int steps, int interval, int startRecord, int run)
{
    //double normRPS = 1.0 + mobilityRateRPS;
    //double normML = 2.0 + mobilityRate;

    //double fracRPS = (1.0 * interfaceDistance) / sizeY;

    //double norm = (fracRPS * normRPS) + ((1 - fracRPS) * normML);

    int p = sizeX * sizeY;
    int halfX = sizeX / 2;
    int rMax = halfX;
    int rMin = (-1 * halfX) - 1;

    //double deltaT = 1.0 / (norm * p);

    double deltaT = 1.0 / p;

    int timesteps = (steps - startRecord) / interval;
    //int idx = 0;

    //double** temporalData = new double*[sizeY];
    //double* temporalData = new double[timesteps];
    //double** temporalData = new double[timesteps];
    //for (int y = 0; y < sizeY; y++)
    //{
        //temporalData[y] = new double[timesteps];
    //}

    //int* times = new int[timesteps];

    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps);

    cout << "Starting Monte Carlo Run" << endl;
    updateDensity();
    //do

    double invTimesteps = 1.0 / (1.0 * timesteps);
    double ** avgAC = new double*[256];
    double ** avgDen = new double*[4];

    for (int i = 0; i < 4; ++i) 
    {
        avgDen[i] = new double[sizeY];
    }

    for (int y = 0; y < 256; y++)
    {
        avgAC[y] = new double[256];
        for (int r = 0; r < 256; r++)
        {
            avgAC[y][r] = 0.0;
        }
    }

    while (monteCarloStep < steps)
    {
        if (monteCarloStep % interval == 0 && timestep == 0.0)
        {
            float progress = (1.0 * monteCarloStep) / (steps - 1);
            progressBar(progress);

            if (run == 0)
            {
                dataOutput(0);
            }

            if (monteCarloStep >= startRecord)
            {
                for (int y = 0; y < 256; y++)
                {
                    for (int r = 0; r < 256; r++)
                    {
                        avgAC[y][r] += (autoCorrelator(0, y, r, 1) * invTimesteps);
                    }
                }
                for (int y = 0; y < sizeY; y++)
                {
                    for (int i = 0; i < 3; ++i) 
                    {
                        avgDen[i][y] += (density1[i][y] * invTimesteps);
                    }
                    double currDen = density0[0][y] + density1[1][y] + density1[2][y];
                    avgDen[3][y] += (currDen * invTimesteps);
                }
                //if (run == 0)
                //{
                    // Writes the current lattice state to a csv
                    //dataOutput(0);
                //}


                // Writes the current population density
                //dataOutput(1);

                
                //outputAutoCorr(0, rMin, rMax, 1);
                //outputAutoCorr(3, 1, 1);
            }


                // Writes the currrent binned predation rate
                //dataOutput(5);

                // Writes the currrent binned breeding rate
                //dataOutput(6);

                // Writes the currrent binned diffusion rate
                //dataOutput(7);

                //if (idx < timesteps)
                //{
                    //times[idx] = monteCarloStep;

                    //double A = 0.0;
                    //for (int yIdx = 0; yIdx < sizeY; yIdx++)
                    //{
                        //temporalData[yIdx][idx] = density1[0][yIdx];
                    //}
                    
                    //temporalData[idx] = globalDensity();
                    //double B = 0.0;
                    //double C = 0.0;
                    //A = A/sizeY;
                    //B = B/sizeY;
                    //C = C/sizeY;
                    //temporalData[idx] = (A + B + C);
                    //double ac;
                    //for (int y = 0; y < sizeY; y++)
                    //{
                        //for (int r = 0; r < halfX ; r++)
                        //{
                            //ac = autoCorrelator(0, y, r, 1) + 
                                 //autoCorrelator(1, y, r, 1) +
                                 //autoCorrelator(2, y, r, 1);
                                
                                 //autoCorrelator(1, y, r) + 
                                 //autoCorrelator(2, y, r);
                            //autoCorr[idx][y][r] = ac / 3;
                        //}
                    //}
                //}
                //idx ++;
            //}
            //clearBinnedReactionCount();
        }
        
        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep += deltaT;

        //do 
        while (latt[x][y].getSpecies() > 2)
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep += deltaT;
        }
        //while (latt[x][y].getSpecies() > 2);
 
        // Decides whether to use reaction(x, y) or reactionRPS(x, y)
        if ( (y >= RPSMin && y < RPSMax) ) 
        {
            if (topology == 1)
            {
                RPSReaction(x, y);
            }
            else if (x >= RPSMin && x <= RPSMax)
            {
                switch(orientation)
                {
                    //case 0  :   RPSReaction(x, y); break;
                    //case 1  :   reaction(x, y); break;
                    //default :   RPSReaction(x, y); break;
                    case 0  :   reaction(x, y); break;
                    case 1  :   reaction(x, y); break;
                    default :   reaction(x, y); break;
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

        if (timestep >= 1.0) 
        {
            timestep = 0.0;

            updateDensity();
        
            monteCarloStep++;
        }

    }
    //while (monteCarloStep < steps);

    //cout << endl << "Simulation Complete" << endl;

    stringstream ss;
    ss << filePath << "correlation_function.tsv";
    string fileName;
    fileName = ss.str();

    fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    cout << "writing auto-correlation data" << endl;
    for (int y = 0; y < 256; y++)
    {
        for (int r = 0; r < 256; r++)
        {
            //data << temporalData[t];
            data << avgAC[y][r];
            if (r < 255)
            {
                data << ",";
            }
        }
        if (y < 255)
        {
            data << endl;
        }
    }

    data.close();

    for (int y = 0; y < 256; y++)
    {
        delete[] avgAC[y];
    }
    delete[] avgAC;

    ss.str("");
    ss << filePath << "average_density.tsv";
    fileName = ss.str();

    fstream data2(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    for (int i = 0; i < 4; ++i) 
    {
        for (int y = 0; y < sizeY; ++y) 
        {
            data2 << avgDen[i][y];

            if (y < sizeY - 1) 
            {
                data2 << ",";
            }
        }
        if (i < 3) 
        {
            data2 << endl;
        }
    }

    for (int i = 0; i < 4; ++i) 
    {
        delete[] avgDen[i];
    }
    delete[] avgDen;

    data2.close();
    //for (int t = 0; t < timesteps; t++)
    //{
        //ss.str("");
        //ss << filePath << "autoCorr_" << times[t] << ".csv";
        //string fileName2 = ss.str();

        //fstream data2(fileName2.c_str(), ofstream::out | ofstream::app | ofstream::in);

        //for (int y = 0; y < sizeY; y++)
        //{
            //for (int r = 0; r < halfX; r++)
            //{
                //data2 << autoCorr[t][y][r];
                //if (r < halfX - 1)
                //{
                    //data2 << ",";
                //}
            //}
            //if (y < sizeY - 1)
            //{
                //data2 << endl;
            //}
        //}

        //cout << "?" << endl;
        //data2.close();
        //cout << "??" << endl;
    //}


    /*
    cout << "writing normSpecData" << endl;
    ss.str("");
    ss << filePath << "normSpectralData.csv";
    string fileName2 = ss.str();

    fstream data2(fileName2.c_str(), ofstream::out | ofstream::app | ofstream::in);

    for (int y = 0; y < sizeY; y++)
    {
        for (int t = 0; t < timesteps; t++)
        {
            data2 << normSpecData[y][t];
            if (t < timesteps - 1)
            {
                data2 << ",";
            }
        }
        if (y < sizeY - 1)
        {
            data2 << endl;
        }
    }

    data2.close();

    */

    //cout << "?" << endl;

    //cout << "??" << endl;
    //for (int i = 0; i < timesteps; i++)
    //{
        //cout << "i=" << i;
        //for (int j = 0; j < sizeY; j++)
        //{
            //delete[] autoCorr[i][j];
        //}
        //cout << "!" << endl;
        //delete[] autoCorr[i];
    //}
    //cout << "???" << endl;
    //delete[] autoCorr;
    //cout << "????" << endl;

    //delete[] times;

    cout << endl << "Simulation Complete" << endl;
}
