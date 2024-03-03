#ifndef SIMULATED_ANNEALING_ATSP_H
#define SIMULATED_ANNEALING_ATSP_H


#include <iostream>
#include <vector>
#include <set>
#include <limits>

using namespace std;

// Wszystkie metody i ich działanie opisano w pliku ATSP.cpp

class ATSP {
public:
    void initializeDistanceMatrix(const int& newDimension);

    void clearDistanceMatrix();

    void printDistanceMatrix();

    void loadATSPFile(const string& fileName);

    void simulatedAnnealing(const string &maxTime,
                            const string &initialTempMethod,
                            const string &calcNewTempMethod,
                            const string &finalTemp,
                            const string &initialSolutionMethod,
                            const string &newSolutionMethod,
                            const string &lenOfAge,
                            const string &alphaParameter,
                            const string& betaParameter);

private:
    // Zmienna określająca rozmiar problemu (liczbę miast)
    int V;

    // Macierz przechowująca odległości między miastami
    vector<vector<int>> distanceMatrix;


    void generateGreedyInitialSolution(vector<int>& solution);

    void generateRandomInitialSolution(vector<int>& solution);


    double initTempAvgDistance();

    double initTempMaxDistance();

    double initTempSampling();


    void swapElements(vector<int>& solution, int i, int j);

    void insertElement(vector<int>& solution, int i, int j);

    void invertElements(vector<int>& solution, int i, int j);


    void calculateNewTemperatureGeo(double &currentTemperature, double alpha);

    void calculateNewTemperatureLog(double& currentTemperature, int temperatureChangeCounter);

    void calculateNewTemperatureLine(double &currentTemperature, double beta);


    int calculateCost(const vector<int>& solution);

    static int generateRandomNumber(int min, int max);

    static double generateRandomDouble();

    static long long int read_QPC();
};


#endif //SIMULATED_ANNEALING_ATSP_H
