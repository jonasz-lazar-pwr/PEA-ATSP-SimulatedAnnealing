#include <windows.h>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <cmath>

#include "ATSP.h"

// Funkcja inicjalizująca macierz odległości
void ATSP::initializeDistanceMatrix(const int& newDimension) {
    // Uaktualnienie nowego rozmiaru macierzy odległości
    V = newDimension;

    // Inicjalizacja macierzy o odpowiednich wymiarach
    distanceMatrix = vector<vector<int>>(V, vector<int>(V));
}

// Funkcja pomocnicza służąca do czyszczenia macierzy
void ATSP::clearDistanceMatrix() {
    // Zerowanie rozmiaru macierzy odległości
    V = 0;

    // Czyszczenie macierzy odległości
    for (auto& innerVector : distanceMatrix) {
        innerVector.clear();
    }
    distanceMatrix.clear();
}

// Funkcja wyświetlająca zawartość macierzy odległości
void ATSP::printDistanceMatrix() {

    // Ustawienie szerokość wyświetlanego pola
    int fieldWidth = 4;

    // Pętle iterujące po każdym elemencie macierzy
    for (const auto& row: distanceMatrix) {
        for (const auto& distance: row) {
            cout << setw(fieldWidth) << distance;
        }
        cout << endl;
    }
}

// Funkcja służąca do wczytywania pliku ATSP do tablicy dwuwymiarowej
void ATSP::loadATSPFile(const string& fileName) {

    //Otwórz plik do odczytu
    //string filename = "../test_files/" + fileName;
    ifstream file;
    file.open(fileName, ifstream::in);

    if (file.is_open()) {
        string line;

        // Pętla iterująca po każdym wierszu tekstu w pliku
        while (getline(file, line)) {

            // Sprawdzenie, czy w obecnej linii znajduje się informacja o rozmiarze grafu
            if (line.find("DIMENSION: ") != string::npos) {
                size_t pos = line.find(": ");

                // Inicjalizacja macierzy odległości na podstawie rozmiaru
                initializeDistanceMatrix(stoi(line.substr(pos + 1)));

                // Rozpoczęcie procesu wczytywania odległości do macierzy
            } else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
                for (int i = 0; i < V; i++) {
                    for (int j = 0; j < V; j++) {
                        file >> distanceMatrix[i][j];
                        if (i == j) {
                            // Ustawianie -1 na głównej przekątnej
                            distanceMatrix[i][j] = -1;
                        }
                    }
                }
            }
        }
    } else {
        // Komunikat o błędzie w przypadku problemu z otwarciem pliku
        cerr << "ERROR while opening the file." << endl;
    }
    file.close();
}

void ATSP::simulatedAnnealing(const string& maxTime,
                              const string& initialTempMethod,
                              const string& calcNewTempMethod,
                              const string& finalTemp,
                              const string& initialSolutionMethod,
                              const string& newSolutionMethod,
                              const string& lenOfAge,
                              const string& alphaParameter,
                              const string& betaParameter) {

    long long int frequency, startTime, endTime, momentOfFinding;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

    double maxExecutionTime = stod(maxTime);
    int ageLength = stoi(lenOfAge);
    double finalTemperature = stod(finalTemp);
    double alpha = stod(alphaParameter);
    double beta = stod(betaParameter);

    // Początkowe rozwiązanie
    vector<int> currentSolution(V);
    int currentCost;

    // Wygenerowanie rozwiązania początkowego
    if (initialSolutionMethod == "RANDOM") {
        generateRandomInitialSolution(currentSolution);
    } else if (initialSolutionMethod == "GREEDY") {
        generateGreedyInitialSolution(currentSolution);
    }

    // Obliczenie kosztu początkowego rozwiązaniaa
    currentCost = calculateCost(currentSolution);

    // Pamiętanie najlepszego rozwiązania
    vector<int> bestSolution = currentSolution;
    int bestCost = currentCost;

    // Aktualne rozwiązanie
    vector<int> newSolution(V);
    int newCost;

    //Obliczenie temperatury początkowej
    double currentTemperature;
    if (initialTempMethod == "AVG") {
        currentTemperature = initTempAvgDistance();
    } else if (initialTempMethod == "MAX") {
        currentTemperature = initTempMaxDistance();
    } else if (initialTempMethod == "SAMPLE") {
        currentTemperature = initTempSampling();
    }

    int temperatureChangeCounter = 1;
    int acceptedSolutions;
    int notAcceptedSolutions;

    // Początkowy czas wykonania algorytmu
    startTime = read_QPC();

    while ((currentTemperature >= finalTemperature) && (((1.0 * (read_QPC() - startTime)) / frequency) <= maxExecutionTime)) {

        acceptedSolutions = 0;
        notAcceptedSolutions = 0;

        for (int age = 0; age < ageLength; age++) {

            // Określenie nowego rozwiązania
            int i = generateRandomNumber(0, V - 1);
            int j;

            // Zabezpieczenie przed wylosowaniem tych samych indeksów
            do {
                j = generateRandomNumber(0, V - 1);
            } while (i == j);

            newSolution = currentSolution;

            if (newSolutionMethod == "SWAP") {
                swapElements(newSolution, i, j);
            } else if (newSolutionMethod == "INSERT") {
                insertElement(newSolution, i, j);
            } else if (newSolutionMethod == "INVERT") {
                invertElements(newSolution, i, j);
            }

            // Obliczenie kosztu nowego rozwiązania
            newCost = calculateCost(newSolution);

            // Obliczenie delty
            int delta = newCost - currentCost;

            if ((delta < 0) || generateRandomDouble() < min(1.0, exp(-(delta / currentTemperature)))) {
                currentSolution = newSolution;
                currentCost = newCost;
                acceptedSolutions++;

            } else {
                notAcceptedSolutions++;
            }

            // Pamiętanie najlepszego rozwiązania
            if (currentCost < bestCost) {
                bestSolution = currentSolution;
                bestCost = currentCost;
            }

            if ((static_cast<double>(acceptedSolutions) / ageLength) > 0.55) {
                age = ageLength;
            }

            if ((static_cast<double>(notAcceptedSolutions) / ageLength) > 0.75) {
                age = ageLength;
            }
        }

        // Metoda Obniżania temperatury
        if (calcNewTempMethod == "GEO") {
            calculateNewTemperatureGeo(currentTemperature, alpha);
        } else if (calcNewTempMethod == "LOG") {
            calculateNewTemperatureLog(currentTemperature, temperatureChangeCounter);
        } else if (calcNewTempMethod == "LINE") {
            calculateNewTemperatureLine(currentTemperature, beta);
        }

        temperatureChangeCounter++;
    }
    endTime = read_QPC();

    // Wyświetlenie wyników
    cout << "------------------------------" << endl;
    cout << "Najlepsza trasa znaleziona algorytmem SW: ";
    for (int vertex : bestSolution) {
        cout << vertex << " -> ";
    }
    cout << bestSolution[0] << endl;

    cout << "--------------------------------" << endl;
    cout << "Koszt najlepszej trasy: " << bestCost  << endl;
    cout << "--------------------------------" << endl;
    cout << "Temperatura koncowa: " << currentTemperature << endl;
    cout << "--------------------------------" << endl;
    cout << "Czas wykonania: " << ((1.0 * (endTime - startTime)) / frequency) << "s" << endl;
    cout << "--------------------------------" << endl;
    cout << endl;
}

// Funkcja do generowania rozwiązania początkowego za pomocą algorytmu zachłannego
void ATSP::generateGreedyInitialSolution(vector<int>& solution) {
    // Lista miast do odwiedzenia
    set<int> remainingCities;
    for (int i = 1; i < V; ++i) {
        remainingCities.insert(i);
    }

    // Aktualne miasto
    int currentCity = 0;
    solution[0] = currentCity;

    // Iteracja po pozostałych miastach
    for (int i = 1; i < V; ++i) {
        // Znajdź najbliższe miasto
        int nearestCity = -1;
        int minDistance = numeric_limits<int>::max();

        for (int city : remainingCities) {
            if (distanceMatrix[currentCity][city] < minDistance) {
                minDistance = distanceMatrix[currentCity][city];
                nearestCity = city;
            }
        }

        // Dodaj najbliższe miasto do rozwiązania
        solution[i] = nearestCity;
        remainingCities.erase(nearestCity);
        currentCity = nearestCity;
    }
}

void ATSP::generateRandomInitialSolution(vector<int>& solution) {
    // Inicjalizacja wektora kolejnych numerów miast
    for (int i = 0; i < V; i++) {
        solution[i] = i;
    }

    // Inicjalizacja generatora liczb pseudolosowych
    random_device rd;
    mt19937 generator(rd());

    // Mieszanie wektora w losowej kolejności
    shuffle(solution.begin(), solution.end(), generator);
}

double ATSP::initTempAvgDistance() {

    int totalDistance = 0;

    // Pętla sumująca wszystkie odległości
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j) {
                totalDistance += distanceMatrix[i][j];
            }
        }
    }

    // Średni dystans to suma wszystkich odległości podzielona
    // przez liczbę krawędzi do kwadratu
    double averageDistance = static_cast<double>(totalDistance) / ((V * V) - V);

    // Ustawienie Temp początkowej jako średni dystans
    // pomnożony przez liczbę wierzchołków
    return 0.5 * averageDistance;
}

double ATSP::initTempMaxDistance() {

    int maxDistance = 0;

    // Pętla w której następuje wyszukanie maksymalnego połączenia
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j) {
                if (distanceMatrix[i][j] > maxDistance) {
                    maxDistance = distanceMatrix[i][j];
                }
            }
        }
    }

    // Ustawienie temp. początkowej jako maksymalny dystans
    // pomnożony przez liczbę wierzchołków
    return 0.5 * static_cast<double>(maxDistance);
}

// Inicjalizacja temperatury początkowej na podstawie próbkowaniu 100 losowych rozwiązań
// (obliczenie średniej różnicy kosztów między kolejnymi rozwiązaniami)
double ATSP::initTempSampling() {
    // Przechowuje różnice kosztów między kolejnymi rozwiązaniami
    vector<int> costDifferences;

    int numSamples = 100;

    vector<int> currentSolution(V);
    generateRandomInitialSolution(currentSolution);
    int currentCost = calculateCost(currentSolution);

    // Generuj numSamples losowych rozwiązań początkowych
    for (int sample = 0; sample < numSamples; sample++) {

        // Przejdź do następnego losowego rozwiązania
        vector<int> nextSolution(V);
        generateRandomInitialSolution(nextSolution);

        // Oblicz koszt następnego rozwiązania
        int nextCost = calculateCost(nextSolution);

        // Oblicz różnicę kosztów między kolejnymi rozwiązaniami
        int costDifference = abs(nextCost - currentCost);
        costDifferences.push_back(costDifference);

        currentSolution = nextSolution;
        currentCost = nextCost;
    }

    // Oblicz średnią różnicę kosztów
    int totalDifference = 0;
    for (int difference : costDifferences) {
        totalDifference += difference;
    }

    double averageDifference = static_cast<double>(totalDifference) / numSamples;

    // Ustaw temperaturę początkową na podstawie średniej
    // różnicy kosztów i liczbie miast
    return 0.5 * averageDifference;
}

void ATSP::calculateNewTemperatureGeo(double& currentTemperature, double alpha) {

    currentTemperature = currentTemperature * alpha;
}

void ATSP::calculateNewTemperatureLog(double& currentTemperature, int temperatureChangeCounter) {

    currentTemperature = (currentTemperature / log10(1 + temperatureChangeCounter));
}

void ATSP::calculateNewTemperatureLine(double& currentTemperature, double beta) {

    currentTemperature = (currentTemperature / (1 + (beta * currentTemperature)));
}

// Funkcja do obliczania kosztu rozwiązania
int ATSP::calculateCost(const vector<int>& solution) {
    int cost = 0;

    for (int i = 0; i < V - 1; ++i) {
        cost += distanceMatrix[solution[i]][solution[i + 1]];
    }

    // Dodanie kosztu powrotu do pierwszego miasta (cyklu)
    cost += distanceMatrix[solution[V - 1]][solution[0]];

    return cost;
}

// Funkcja zamieniająca miejscami dwa elementy w rozwiązaniu
void ATSP::swapElements(vector<int>& solution, int i, int j) {

    // Zamień miejscami elementy o podanych indeksach w wektorze rozwiązania
    swap(solution[i], solution[j]);
}

// Funkcja do wstawiania elementu w rozwiązaniu
void ATSP::insertElement(vector<int>& solution, int i, int j) {

    // Wartość elementu do wstawienia
    int valueToInsert = solution[i];

    // Usunięcie elementu z pozycji i
    solution.erase(solution.begin() + i);

    // Wstawienie elementu na pozycję j
    solution.insert(solution.begin() + j, valueToInsert);
}

// Funkcja do odwracania elementów w rozwiązaniu
void ATSP::invertElements(vector<int>& solution, int i, int j) {

    // Znalezienie minimum i maksimum spośród i i j
    int minIndex = min(i, j);
    int maxIndex = max(i, j);

    // Odwrócenie kolejności elementów między minIndex a maxIndex włącznie
    reverse(solution.begin() + minIndex, solution.begin() + maxIndex + 1);
}

int ATSP::generateRandomNumber(int min, int max) {
    // Inicjalizacja generatora liczb pseudolosowych
    random_device rd;
    mt19937 generator(rd());

    // Definicja rozkładu prawdopodobieństwa z zakresu [min, max]
    uniform_int_distribution<> distribution(min, max);

    // Wygenerowanie liczby zmiennoprzecinkowej z zakresu [min, max]
    return distribution(generator);
}

// Funkcja do generowania liczby zmiennoprzecinkowej z zakresu [0, 1)
double ATSP::generateRandomDouble() {
    // Inicjalizacja generatora liczb pseudolosowych
    random_device rd;
    mt19937_64 generator(rd());

    // Definicja rozkładu prawdopodobieństwa z zakresu [0, 1)
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // Wygenerowanie liczby zmiennoprzecinkowej z zakresu [0, 1)
    return distribution(generator);
}

long long int ATSP::read_QPC() {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}
