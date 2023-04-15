#ifndef __FILESTREAM__
#define __FILESTREAM__

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <sstream>

using namespace std;

// Define a Data structure type to store my values
typedef struct {
    vector<double> X;
    vector<double> Y;
    vector<double> dX;
    vector<double> dY;
} Data;

/**
 * @brief Generate quadratic function with errors
 * 
 * @param filename: path of data file 
 */
void WriteDAT(const string &filename) {
    cout << "Writting data..." << endl;
    ofstream file;
    // Open and verify if it was opened correctly
    file.open(filename);
    if (!file.is_open()) {
        fprintf(stderr, "**Can not open DAT file to write\n");
        exit(0);
    }
    // Write headers
    file << "X " << "Y " << "dX " << "dY" << endl;

    TRandom3 *rnd = new TRandom3(0);
    double dX = 0.5;
    double dY;

    // Write each element in a column
    for (int i = -5; i < 6; i++) {
        float error = (2*rnd->Rndm()-1);
        dY = sqrt(abs(i));
        file << i << " " << pow(i,2) + error << " " << dX << " " << dY << endl;
    }
    file.close();
}

/**
 * @brief Read .dat file and return stored values
 * 
 * @param filename: path of data file 
 * @return Data 
 */
Data ReadDAT(const string &filename) {
    cout << "Reading data...\n";
    Data data;

    ifstream inFile(filename);
    if (!inFile.is_open()) {
        fprintf(stderr, "**Can not open DAT file to read\n");
        exit(0);
    }
// Read header lines and discard them
    string line;
    getline(inFile, line);

    // Read data lines and parse them
    double col1, col2, col3, col4;
    while (inFile >> col1 >> col2 >> col3 >> col4) {
        data.X.push_back(col1);
        data.Y.push_back(col2);
        data.dX.push_back(col3);
        data.dY.push_back(col4);
        cout << col1 << " " << col2 << " " << col3 << " " << col4 << endl;
    }
    inFile.close();
    return data;
}
#endif