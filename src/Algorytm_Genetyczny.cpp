#include "Algorytm_Genetyczny.h"
#include "Operatory_GA.h"
#include <numeric>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

Osobnik Algorytm_Genetyczny::wykonaj(const vector<vector<int>>& graf, 
                                     const WczytywanieKonfiguracji& config, 
                                     const string& plik_konwergencji) {
                                         
    int n = graf.size();
    vector<Osobnik> populacja(config.ga_rozmiar_populacji);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> szansa(0.0, 1.0);

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

    for (int pokolenie = 0; pokolenie < config.ga_max_pokolen; ++pokolenie) {
        
        sort(populacja.begin(), populacja.end());
        
        if (populacja[0].koszt < globalnie_najlepszy.koszt) {
            globalnie_najlepszy = populacja[0];
        }

        if (plik_log.is_open()) {
            plik_log << pokolenie << ";" << globalnie_najlepszy.koszt << "\n";
        }

        vector<Osobnik> nowa_populacja;

        for (int i = 0; i < config.ga_elitaryzm; ++i) {
            nowa_populacja.push_back(populacja[i]);
        }

        while (nowa_populacja.size() < (size_t)config.ga_rozmiar_populacji) {
            
            int idx1 = Operatory_GA::selekcja_turniejowa(populacja, config.ga_rozmiar_turnieju);
            int idx2 = Operatory_GA::selekcja_turniejowa(populacja, config.ga_rozmiar_turnieju);
            
            Osobnik dziecko1 = populacja[idx1];
            Osobnik dziecko2 = populacja[idx2];

            if (szansa(gen) < config.ga_wsp_krzyzowania) {
                if (config.ga_metoda_krzyzowania == "OX") {
                    Operatory_GA::krzyzowanie_OX(populacja[idx1], populacja[idx2], dziecko1, dziecko2);
                } else if (config.ga_metoda_krzyzowania == "PMX") {
                    Operatory_GA::krzyzowanie_PMX(populacja[idx1], populacja[idx2], dziecko1, dziecko2);
                }
}

            if (szansa(gen) < config.ga_wsp_mutacji) {
                if (config.ga_metoda_mutacji == "SCRAMBLE") {
                    Operatory_GA::mutacja_scramble(dziecko1);
                } else {
                    Operatory_GA::mutacja_inwersja(dziecko1);
                }
            }

            if (szansa(gen) < config.ga_wsp_mutacji) {
                if (config.ga_metoda_mutacji == "SCRAMBLE") {
                    Operatory_GA::mutacja_scramble(dziecko2);
                } else {
                    Operatory_GA::mutacja_inwersja(dziecko2);
                }
            }

            dziecko1.koszt = Operatory_GA::oblicz_koszt(dziecko1.trasa, graf);
            nowa_populacja.push_back(dziecko1);

            if (nowa_populacja.size() < (size_t)config.ga_rozmiar_populacji) {
                dziecko2.koszt = Operatory_GA::oblicz_koszt(dziecko2.trasa, graf);
                nowa_populacja.push_back(dziecko2);
            }
        }

        populacja = nowa_populacja;
    }
    
    if (plik_log.is_open()) plik_log.close();
    
    sort(populacja.begin(), populacja.end());
    if (populacja[0].koszt < globalnie_najlepszy.koszt) {
        globalnie_najlepszy = populacja[0];
    }

    return globalnie_najlepszy;
}