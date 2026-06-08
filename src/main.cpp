#include <iostream>
#include <ctime>
#include "Wczytywanie_macierzy.h"
#include "Algorytm_Genetyczny.h"

using namespace std;

int main() {
    srand(time(NULL)); // Inicjalizacja ziarna losowości

    // Zmień ścieżkę na swój dowolny plik z miastami (np. brazil58 lub gr17)
    string sciezka = "dane/tsp_proste/sym/gr17.tsp_prosty.txt"; 
    
    cout << "Wczytywanie macierzy: " << sciezka << "...\n";
    Macierz mat = Wczytywanie_Macierzy::wczytajMacierz(sciezka);
    
    if (mat.dane.empty()) {
        return 1;
    }

    cout << "Uruchamianie Algorytmu Genetycznego...\n";
    cout << "Optimum (jesli znane): " << mat.optymalnyKoszt << "\n\n";

    // Parametry na sztywno do testu
    int rozmiar_populacji = 100;
    int max_pokolen = 1000;
    double wsp_mutacji = 0.05;      // 5%
    double wsp_krzyzowania = 0.8;   // 80%

    Osobnik najlepszy = uruchom_GA(mat, rozmiar_populacji, max_pokolen, wsp_mutacji, wsp_krzyzowania);

    cout << "\n=== KONIEC ===\n";
    cout << "Najlepszy znaleziony koszt: " << najlepszy.koszt << "\n";

    return 0;
}