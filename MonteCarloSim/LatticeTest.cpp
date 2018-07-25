#include "Lattice.hpp"

#include <iostream>
#include <string>

void progressBar(float progress)
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

int main(int argc, char *argv[])
{
    string path = string(argv[1]).append("/");
    int runs = 50;
    int size = 256;
    double mobilities[] = {0.1, 1.25, 2.5, 5.0, 10.0};
    //int size = atoi(argv[2]);
    //double mobility = atof(argv[3]);
    int binSize = 4;
    int steps = 1000;
    //int steps = atoi(argv[4]);
    int totalMCRuns = runs * 5;

    stringstream ss;
    ss << path << "densities.csv";
    string filename = ss.str();
    fstream data(filename.c_str(), ofstream::out | ofstream::app | ofstream::in);
    fstream mob_data(filename.c_str(), ofstream::out | ofstream::app | ofstream::in);

    cout << "beginning test" << endl;

    int prog = 0;

    for (int i = 0; i < 5; i++)
    {
        mob_data << mobilities[i] << ",";
        for (int run = 0; run < runs; run++)
        {
            float progress = (1.0 * prog) / totalMCRuns;
            progressBar(progress);
            double mobility = mobilities[i];
            Lattice testLattice(path, size, mobility, binSize);
            double density = testLattice.densityRun(steps);
            data << density << ",";
            prog++;
            progress = (1.0 * prog) / totalMCRuns;
            progressBar(progress);
        }
        data << endl;
    }

    data.close();
    mob_data.close();

    cout << "test complete" << endl;
    return 0;
}
