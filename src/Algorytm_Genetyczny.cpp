#include "Algorytm_Genetyczny.h"
#include "Operatory_GA.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>

using namespace std;

Osobnik uruchom_GA(const Macierz& macierz, int rozmiar_populacji, int max_pokolen, double wsp_mutacji, double wsp_krzyzowania) {
    int n = macierz.rozmiar;
    vector<Osobnik> populacja(rozmiar_populacji);

    // 1. Inicjalizacja populacji początkowej (losowe permutacje)
    vector<int> bazowa_trasa(n);
    iota(bazowa_trasa.begin(), bazowa_trasa.end(), 0);

    for (int i = 0; i < rozmiar_populacji; ++i) {
        random_shuffle(bazowa_trasa.begin(), bazowa_trasa.end());
        populacja[i].trasa = bazowa_trasa;
        populacja[i].koszt = oblicz_koszt_ga(bazowa_trasa, macierz);
    }

    sort(populacja.begin(), populacja.end());
    Osobnik globalnie_najlepszy = populacja[0];

    // 2. Pętla pokoleń
    for (int pokolenie = 0; pokolenie < max_pokolen; ++pokolenie) {
        vector<Osobnik> nowa_populacja;
        
        // Elitaryzm - dodaj najlepszego z poprzedniego pokolenia
        nowa_populacja.push_back(populacja[0]);

        // Generowanie reszty populacji
        while (nowa_populacja.size() < rozmiar_populacji) {
            // Selekcja rodziców
            Osobnik rodzic1 = selekcja_turniejowa(populacja, 5);
            Osobnik rodzic2 = selekcja_turniejowa(populacja, 5);

            Osobnik dziecko;
            
            // Krzyżowanie
            if ((rand() % 100) < (wsp_krzyzowania * 100)) {
                dziecko.trasa = krzyzowanie_OX(rodzic1.trasa, rodzic2.trasa);
            } else {
                dziecko.trasa = rodzic1.trasa; // Kopia rodzica jeśli nie zaszło krzyżowanie
            }

            // Mutacja
            if ((rand() % 100) < (wsp_mutacji * 100)) {
                mutacja_inwersja(dziecko.trasa);
            }

            dziecko.koszt = oblicz_koszt_ga(dziecko.trasa, macierz);
            nowa_populacja.push_back(dziecko);
        }

        populacja = nowa_populacja;
        sort(populacja.begin(), populacja.end());

        if (populacja[0].koszt < globalnie_najlepszy.koszt) {
            globalnie_najlepszy = populacja[0];
        }

        // Wypisz postęp co 50 pokoleń (aby sprawdzić, czy działa)
        if (pokolenie % 50 == 0) {
            cout << "Pokolenie " << pokolenie << " | Najlepszy koszt: " << globalnie_najlepszy.koszt << "\n";
        }
    }

    return globalnie_najlepszy;
}