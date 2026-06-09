#include <iostream>
#include "WczytywanieKonfiguracji.h"
#include "Testy_GA.h"

using namespace std;

int main() {
    WczytywanieKonfiguracji config;
    if (!config.wczytaj_z_pliku("config.txt")) {
        cerr << "Krytyczny blad inicjalizacji pliku konfiguracyjnego." << endl;
        return 1;
    }

    cout << "===========================================" << endl;
    cout << "PEA Projekt 4: Algorytm Genetyczny dla TSP" << endl;
    cout << "===========================================" << endl;
    cout << "Badany plik: " << config.sciezka_do_pliku << endl;

    przeprowadz_testy_ga(config.sciezka_do_pliku, config);

    return 0;
}