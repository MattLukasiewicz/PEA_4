#include <iostream>
#include <ctime>
#include "Wczytywanie_macierzy.h"
#include "Algorytm_Genetyczny.h"

using namespace std;

int main() {
    srand(time(NULL)); // Inicjalizacja ziarna losowości

    // Domyślna ścieżka (jeśli plik nie istnieje, spróbujemy pliku testowego)
    string sciezka = "dane/tsp_proste/sym/gr17.tsp_prosty.txt";
    cout << "Wczytywanie macierzy: " << sciezka << "...\n";
    Macierz mat = Wczytywanie_Macierzy::wczytajMacierz(sciezka);

    if (mat.dane.empty()) {
        string fallback = "dane/sample_simple.txt";
        cout << "Plik domyślny nie istnieje. Probuje: " << fallback << "...\n";
        mat = Wczytywanie_Macierzy::wczytajMacierz(fallback);
        if (mat.dane.empty()) {
            cerr << "Brak pliku z danymi. Umieść plik w katalogu 'dane' i zmień sciezke w main.cpp.\n";
            return 1;
        }
    }

    cout << "Uruchamianie Algorytmu Genetycznego...\n";
    cout << "Optimum (jesli znane): " << mat.optymalnyKoszt << "\n\n";

    // Parametry na sztywno do testu
    size_t rozmiar_populacji = 100;
    int max_pokolen = 1000;
    double wsp_mutacji = 0.05;      // 5%
    double wsp_krzyzowania = 0.8;   // 80%

    Osobnik najlepszy = uruchom_GA(mat, rozmiar_populacji, max_pokolen, wsp_mutacji, wsp_krzyzowania);

    cout << "\n=== KONIEC ===\n";
    cout << "Najlepszy znaleziony koszt: " << najlepszy.koszt << "\n";

    return 0;
}