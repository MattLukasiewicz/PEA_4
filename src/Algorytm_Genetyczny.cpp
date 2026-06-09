#include "Algorytm_Genetyczny.h"
#include "Operatory_GA.h"
#include <numeric>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

Osobnik Algorytm_Genetyczny::wykonaj(const vector<vector<int>>& graf, 
                                     const WczytywanieKonfiguracji& cfg, 
                                     const string& plik_konwergencji) {
                                         
    int n = graf.size();
    vector<Osobnik> populacja(cfg.ga_rozmiar_populacji);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> szansa(0.0, 1.0);

    // 1. Inicjalizacja populacji początkowej (całkowicie losowe trasy)
    for (auto& osobnik : populacja) {
        osobnik.trasa.resize(n);
        iota(osobnik.trasa.begin(), osobnik.trasa.end(), 0);
        shuffle(osobnik.trasa.begin(), osobnik.trasa.end(), gen);
        osobnik.koszt = Operatory_GA::oblicz_koszt(osobnik.trasa, graf);
    }

    Osobnik globalnie_najlepszy = populacja[0];

    ofstream plik_log;
    if (!plik_konwergencji.empty()) {
        plik_log.open(plik_konwergencji, ios::app);
    }

    // 2. Główna pętla ewolucji
    for (int pokolenie = 0; pokolenie < cfg.ga_max_pokolen; ++pokolenie) {
        
        // Sortowanie populacji, aby wyłonić elitę i najlepszego osobnika
        sort(populacja.begin(), populacja.end());
        
        if (populacja[0].koszt < globalnie_najlepszy.koszt) {
            globalnie_najlepszy = populacja[0];
        }

        if (plik_log.is_open()) {
            plik_log << pokolenie << ";" << globalnie_najlepszy.koszt << "\n";
        }

        vector<Osobnik> nowa_populacja;

        // Elitaryzm - dodajemy bezwzględnie najlepszego bez żadnych zmian
        for (int i = 0; i < cfg.ga_elitaryzm; ++i) {
            nowa_populacja.push_back(populacja[i]);
        }

        // Generowanie reszty potomstwa
        while (nowa_populacja.size() < (size_t)cfg.ga_rozmiar_populacji) {
            
            // a) Selekcja Turniejowa (rozmiar turnieju sterowany z configu)
            int idx1 = Operatory_GA::selekcja_turniejowa(populacja, cfg.ga_rozmiar_turnieju);
            int idx2 = Operatory_GA::selekcja_turniejowa(populacja, cfg.ga_rozmiar_turnieju);
            
            Osobnik dziecko1 = populacja[idx1];
            Osobnik dziecko2 = populacja[idx2];

            if (szansa(gen) < cfg.ga_wsp_krzyzowania) {
                if (cfg.ga_metoda_krzyzowania == "OX") {
                    Operatory_GA::krzyzowanie_OX(populacja[idx1], populacja[idx2], dziecko1, dziecko2);
                } else if (cfg.ga_metoda_krzyzowania == "PMX") {
                    Operatory_GA::krzyzowanie_PMX(populacja[idx1], populacja[idx2], dziecko1, dziecko2);
                }
}

                        // c) Mutacja dla dziecka 1
            if (szansa(gen) < cfg.ga_wsp_mutacji) {
                if (cfg.ga_metoda_mutacji == "SCRAMBLE") {
                    Operatory_GA::mutacja_scramble(dziecko1);
                } else {
                    Operatory_GA::mutacja_inwersja(dziecko1);
                }
            }

            // d) Mutacja dla dziecka 2
            if (szansa(gen) < cfg.ga_wsp_mutacji) {
                if (cfg.ga_metoda_mutacji == "SCRAMBLE") {
                    Operatory_GA::mutacja_scramble(dziecko2);
                } else {
                    Operatory_GA::mutacja_inwersja(dziecko2);
                }
            }

            // e) Ocena i dodanie do nowej populacji
            dziecko1.koszt = Operatory_GA::oblicz_koszt(dziecko1.trasa, graf);
            nowa_populacja.push_back(dziecko1);

            if (nowa_populacja.size() < (size_t)cfg.ga_rozmiar_populacji) {
                dziecko2.koszt = Operatory_GA::oblicz_koszt(dziecko2.trasa, graf);
                nowa_populacja.push_back(dziecko2);
            }
        }

        populacja = nowa_populacja;
    }
    
    if (plik_log.is_open()) plik_log.close();
    
    // Ostateczne sprawdzenie po wyjściu z pętli
    sort(populacja.begin(), populacja.end());
    if (populacja[0].koszt < globalnie_najlepszy.koszt) {
        globalnie_najlepszy = populacja[0];
    }

    return globalnie_najlepszy;
}