#include <iostream>

#include "Interface.h"
#include "ATSP.h"


void Interface::menu() {

    ATSP atsp;

    char menuOption;
    string fileName;

    string maxTime;
    string initialTempMethod;
    string calcNewTempMethod;
    string finalTemp;
    string initialSolutionMethod;
    string newSolutionMethod;
    string lenOfAge;
    string alphaParameter;
    string betaParameter;

    do {
        cout << endl << "-----MENU GLOWNE-----\n";
        cout << "[1] Wczytanie danych z pliku\n";
        cout << "[2] Ustawienie parametrow\n";
        cout << "[3] Wyswietlenie ostatnio wczytanych danych\n";
        cout << "[4] Algorytm Symulowanego Wyzarzania (SW)\n";
        cout << "[0] Wyjscie\n";

        cout << "Twoj wybor:";
        cin >> menuOption;

        switch (menuOption) {
            case '0':
                cout << "\nWyjscie\n";
                break;
            case '1':
                cout << "\nOpcja 1: Wczytanie danych z pliku\n";

                // Pliki muszą znajdować się w tym samym folderze co plik .exe,
                // oraz muszą mieć charakterystyczną budowę zgodą ze specyfikacją określoną
                // na stronie: http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/atsp/
                cout << "Podaj nazwe pliku wraz z rozszerzeniem (np. ftv47.atsp):";
                cin >> fileName;
                atsp.loadATSPFile(fileName);
                break;
            case '2':
                char parameterOption;
                char initialTempOption;
                char calcNewTempOption;
                char initialSolutionOption;
                char newSolutionOption;

                do {
                    cout << endl << "\n-----USTAWIENIE PARAMETROW-----\n";
                    cout << "[1] Maksymalny czas wykonania\n";
                    cout << "[2] Temperatura poczatkowa\n";
                    cout << "[3] Obnizanie temperatury\n";
                    cout << "[4] Temperatura koncowa\n";
                    cout << "[5] Rozwiazenie poczatkowe\n";
                    cout << "[6] Generowanie nowego rozwiazania\n";
                    cout << "[7] Dlugosc ery\n";
                    cout << "[8] Parametr alfa\n";
                    cout << "[9] Parametr beta\n";
                    cout << "[0] Wyjscie\n";

                    cout << "Twoj wybor:";
                    cin >> parameterOption;

                    switch (parameterOption) {
                        case '0':
                            cout << "\nWyjscie\n";
                            break;
                        case '1':
                            cout << "\nOpcja 1: Maksymalny czas wykonania\n";
                            cout << "Podaj maksymalny czas wykonania [s] (np. 60.0):";
                            cin >> maxTime;
                            break;
                        case '2':
                            cout << "\nOpcja 2: Temperatura poczatkowa\n";
                            cout << "[1] Na podstawie sredniego dystansu\n";
                            cout << "[2] Na podstawie maksymalnego dystansu\n";
                            cout << "[3] Na podstawie probkowania losowych rozwiazan\n";

                            cout << "Twoj wybor:";
                            cin >> initialTempOption;

                            if (initialTempOption == '1') {
                                initialTempMethod = "AVG";
                            } else if (initialTempOption == '2') {
                                initialTempMethod = "MAX";
                            } else {
                                initialTempMethod = "SAMPLE";
                            }
                            break;
                        case '3':
                            cout << "\nOpcja 3: Obnizanie temperatury\n";
                            cout << "[1] Metoda Geometryczna\n";
                            cout << "[2] Metoda Logarytmiczna\n";
                            cout << "[3] Metoda Liniowa\n";

                            cout << "Twoj wybor:";
                            cin >> calcNewTempOption;

                            if (calcNewTempOption == '1') {
                                calcNewTempMethod = "GEO";
                            } else if (calcNewTempOption == '2') {
                                calcNewTempMethod = "LOG";
                            } else {
                                calcNewTempMethod = "LINE";
                            }
                            break;
                        case '4':
                            cout << "\nOpcja 4: Temperatura koncowa\n";
                            cout << "Podaj temerature koncowa (np. 0.0001):";
                            cin >> finalTemp;
                            break;
                        case '5':
                            cout << "\nOpcja 5: Rozwiazenie poczatkowe\n";
                            cout << "[1] Metoda losowa\n";
                            cout << "[2] Metoda zachlanna\n";

                            cout << "Twoj wybor:";
                            cin >> initialSolutionOption;

                            if (initialSolutionOption == '1') {
                                initialSolutionMethod = "RANDOM";
                            } else {
                                initialSolutionMethod = "GREEDY";
                            }
                            break;
                        case '6':
                            cout << "\nOpcja 6: Generowanie nowego rozwiazania\n";
                            cout << "[1] Metoda SWAP\n";
                            cout << "[2] Metoda INSERT\n";
                            cout << "[3] Metoda INVERT\n";

                            cout << "Twoj wybor:";
                            cin >> newSolutionOption;

                            if (newSolutionOption == '1') {
                                newSolutionMethod = "SWAP";
                            } else if (newSolutionOption == '2') {
                                newSolutionMethod = "INSERT";
                            } else {
                                newSolutionMethod = "INVERT";
                            }
                            break;
                        case '7':
                            cout << "\nOpcja 7: Dlugosc ery\n";
                            cout << "Podaj dlugosc trwania ery (np. 2000):";
                            cin >> lenOfAge;
                            break;
                        case '8':
                            cout << "\nOpcja 8: Parametr alfa\n";
                            cout << "Podaj wartosc parametru alfa (np. 0.99):";
                            cin >> alphaParameter;
                            break;
                        case '9':
                            cout << "\nOpcja 9: Parametr beta\n";
                            cout << "Podaj wartosc parametru beta (np. 0.001):";
                            cin >> betaParameter;
                            break;
                        default:
                            cout << "\nPodana opcja nie istnieje!\n";
                            break;
                    }
                } while (parameterOption != '0');


                break;
            case '3':
                cout << "\nOpcja 3: Wyswietlenie ostatnio wczytanych danych\n";
                atsp.printDistanceMatrix();
                break;
            case '4':
                cout << "\nOpcja 4: Algorytm Symulowanego Wyzarzania (SW)\n";
                atsp.simulatedAnnealing(maxTime,
                                        initialTempMethod,
                                        calcNewTempMethod,
                                        finalTemp,
                                        initialSolutionMethod,
                                        newSolutionMethod,
                                        lenOfAge,
                                        alphaParameter,
                                        betaParameter);
                break;
            default:
                cout << "\nPodana opcja nie istnieje!\n";
                break;
        }
    } while (menuOption != '0');
}
