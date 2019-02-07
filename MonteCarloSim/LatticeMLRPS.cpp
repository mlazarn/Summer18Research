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
        if (latt[i][y].getSpecies() == spec && latt[(i+r) % sizeX][y].getSpecies() == spec)
        {
            sum += 1.0;
        }
    }
    double output;

    if ( mode == 1 )
    {
        output = (sum / sizeX) - (density1[spec][y]*density1[spec][y]);
    }
    else 
    {
        output = (sum / sizeX);
    }

    return output;
}

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
        curr.setSpecies(neighSpec);

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
        if (neighbor.getSpecies() == (curr.getSpecies() + 1) % 3)
        {
            decrementSpeciesCount(neighbor.getSpecies());
            neighbor.setSpecies(curr.getSpecies());
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

    //double deltaT = 1.0 / (norm * p);

    double deltaT = 1.0 / p;

    int timesteps = (steps - startRecord) / interval;
    int idx = 0;

    double** temporalData = new double*[sizeY];
    //double** temporalData = new double[timesteps];
    for (int y = 0; y < sizeY; y++)
    {
        temporalData[y] = new double[timesteps];
    }

    int* times = new int[timesteps];
    double*** autoCorr = new double**[timesteps];
    for (int i = 0; i < timesteps; i++)
    {
        autoCorr[i] = new double*[sizeY];
        for (int j = 0; j < sizeY; j++)
        {
            autoCorr[i][j] = new double[sizeX];
        }
    }

    cout << "Writing metadata" << endl;
    metadata(startRecord, interval, steps);

    cout << "Starting Monte Carlo Run" << endl;
    do
    {
        if (monteCarloStep % interval == 0 && timestep == 0.0)
        {
            float progress = (1.0 * monteCarloStep) / (steps - 1);
            progressBar(progress);

            if (monteCarloStep >= startRecord)
            {
                if (run == 0)
                {
                    dataOutput(0);
                }
                if (idx < timesteps)
                {
                    times[idx] = monteCarloStep;
                    //temporalData[idx] = globalDensity();
                    for (int yIdx = 0; yIdx < sizeY; yIdx++)
                    {
                        temporalData[yIdx][idx] = density1[0][yIdx];
                    }
                    double ac;
                    for (int y = 0; y < sizeY; y++)
                    {
                        for (int r = 0; r < sizeX ; r++)
                        {
                            ac = autoCorrelator(0, y, r, 0) + 
                                 autoCorrelator(1, y, r, 0) +
                                 autoCorrelator(2, y, r, 0);
                                
                                 //autoCorrelator(1, y, r) + 
                                 //autoCorrelator(2, y, r);
                            autoCorr[idx][y][r] = ac / 3; // / 3;
                        }
                    }
                }
                idx ++;
            }

        }
        
        int x = xCoordDist(rng);
        int y = yCoordDist(rng);
        timestep += deltaT;

        do 
        {
            x = xCoordDist(rng);
            y = yCoordDist(rng);
            timestep += deltaT;
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

        if (timestep >= 1.0) 
        {
            timestep = 0.0;

            updateDensity();
        
            monteCarloStep++;
        }

    }
    while (monteCarloStep < steps);

    cout << endl << "Simulation Complete" << endl;

    /*double** spectralData = new double*[sizeY];
    double** normSpecData = new double*[sizeY];
    for (int y = 0; y < sizeY; y++)
    {
        spectralData[y] = new double[timesteps];
        normSpecData[y] = new double[timesteps];
    }

    cout << "performing spectral analysis" << endl;
    for (int y = 0; y < sizeY; y++)
    {
        double *in;
        fftw_complex *out;
        fftw_plan plan;

        in = fftw_alloc_real(timesteps);
        out = fftw_alloc_complex(timesteps);

        for (int t = 0; t < timesteps; t++)
        {
            in[t] = temporalData[y][t];
        }

        plan = fftw_plan_dft_r2c_1d(timesteps, in, out, FFTW_ESTIMATE);

        fftw_execute(plan);
        
        for (int t = 0; t < timesteps; t++)
        {
            spectralData[y][t] = out[t][0];
            normSpecData[y][t] = sqrt(pow(out[t][0], 2) + pow(out[t][1], 2));
        }

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    */
    //cout << "writing temporalData" << endl;

    stringstream ss;
    ss << filePath << "temporalData.csv";
    string fileName;
    fileName = ss.str();

    //fstream data(fileName.c_str(), ofstream::out | ofstream::app | ofstream::in);

    //for (int y = 0; y < sizeY; y++)
    //{
        //for (int t = 0; t < timesteps; t++)
        //{
            //data << temporalData[t];
            //data << temporalData[y][t] - 0.26;
            //if (t < timesteps - 1)
            //{
                //data << ",";
            //}
        //}
        //if (y < sizeY - 1)
        //{
            //data << endl;
        //}
    //}

    //data.close();

    cout << "writing auto-correlation data" << endl;

    for (int t = 0; t < timesteps; t++)
    {
        ss.str("");
        ss << filePath << "autoCorr_" << times[t] << ".csv";
        string fileName2 = ss.str();

        fstream data2(fileName2.c_str(), ofstream::out | ofstream::app | ofstream::in);

        for (int y = 0; y < sizeY; y++)
        {
            for (int r = 0; r < sizeX; r++)
            {
                data2 << autoCorr[t][y][r];
                if (r < sizeX - 2)
                {
                    data2 << ",";
                }
            }
            if (y < sizeY - 1)
            {
                data2 << endl;
            }
        }

        //cout << "?" << endl;
        data2.close();
        //cout << "??" << endl;
    }


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
    for (int y = 0; y < sizeY; y++)
    {
        delete[] temporalData[y];
    }
    delete[] temporalData;

    //cout << "??" << endl;
    for (int i = 0; i < timesteps; i++)
    {
        //cout << "i=" << i;
        for (int j = 0; j < sizeY; j++)
        {
            delete[] autoCorr[i][j];
        }
        //cout << "!" << endl;
        delete[] autoCorr[i];
    }
    //cout << "???" << endl;
    delete[] autoCorr;
    //cout << "????" << endl;
    delete[] times;

    cout << endl << "Simulation Complete" << endl;
}
