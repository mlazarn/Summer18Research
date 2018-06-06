#include "Cell.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    Cell cells[4];
    for (int i = 0; i  < 4; i++)
    {
        cout << i << ":" << endl;
        cout << "Species: " << cells[i].getSpecies() << endl;
        cout << "Predation Rate: " << cells[i].getPredRate() << endl;
        cout << "Fertility Rate: " << cells[i].getFertRate() << endl;
        cout << "Swap Rate: " << cells[i].getSwapRate() << endl;
        cout << "Diffusion Rate: " << cells[i].getDifRate() << endl;
    }

    cout << "reassigning species" << endl;

    for (int i = 0; i  < 4; i++)
    {
        cells[i].setSpecies(i);
    }

    for (int i = 0; i  < 4; i++)
    {
        cout << i << ":" << endl;
        cout << "Species: " << cells[i].getSpecies() << endl;
        cout << "Predation Rate: " << cells[i].getPredRate() << endl;
        cout << "Fertility Rate: " << cells[i].getFertRate() << endl;
        cout << "Swap Rate: " << cells[i].getSwapRate() << endl;
        cout << "Diffusion Rate: " << cells[i].getDifRate() << endl;
    }

    return 0;
}
