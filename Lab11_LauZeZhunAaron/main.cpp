#include "DataIO.h"
#include "Menu.h"
#include <iostream>

using std::cout;

// Load all listed files before the menu so each option uses the same stored readings
// The map and BST hold the data throughout this run, Vectors are only used for temporary processing
// Each module has its own job so loading, storage, maths and console output can be changed separately
int main()
{
    WeatherStore data; // Map of day keys and the BST belonging to each day
    int added = 0;
    int duplicates = 0;
    if(!LoadData("data/data_source.txt", data, added, duplicates)) // Finish loading before starting the menu
    {
        cout << "Unable to load weather data\n";
        return 1;
    }
    cout << "Readings loaded: " << added << '\n';
    cout << "Duplicate timestamps ignored: " << duplicates << '\n';
    RunMenu(data); // Options select readings through the map and BST traversal
    return 0; // The map destroys its trees and their destructors free the nodes
}
