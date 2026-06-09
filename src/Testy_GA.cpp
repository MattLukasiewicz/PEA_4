#include "Testy_GA.h"
#include "Wczytywanie_macierzy.h"
#include "Algorytm_Genetyczny.h"
#include "Stoper.h"
#include "Pasek_postepu.h"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

void przeprowadz_testy_ga(const string& sciezka, const WczytywanieKonfiguracji& config) {
    Macierz macierz = Wczytywanie_Macierzy::wczytajMacierz(sciezka);
    if (macierz.dane.empty()) return;

    vector<vector<int>> graf = macierz.dane;
    int N = graf.size();
    int optimum = (macierz.optymalnyKoszt > 0) ? macierz.optymalnyKoszt : -1;

    if (optimum == -1) {
        cout << "Uwaga: Brak optimum (sum_min) dla: " << sciezka << ". Blad procentowy bedzie N/A." << endl;
    }

    // Przygotowanie pliku wynikowego
    ofstream plik_csv(config.plik_wynikowy, ios::app); 
    plik_csv.seekp(0, ios::end);
    if (plik_csv.tellp() == 0) {
        plik_csv << "Plik;N;Optimum;Powtorzenia;Sredni_Koszt;Sredni_Blad_Proc;Sredni_Czas_ms;Rozmiar_Pop;Pokolenia;Wsp_Krzyz;Metoda_Krzyz;Wsp_Mut;Metoda_Mut;Hybryda2Opt\n";
    }

    Stoper stoper;
    cout << "Rozpoczynam badanie: " << sciezka << " (N=" << N << "), Powtorzenia: " << config.powtorzenia << endl;

    double suma_czasow_ms = 0.0;
    long long suma_kosztow = 0;
    int najlepszy_znaleziony_koszt = numeric_limits<int>::max();

    pokazPostep(config.wyswietlaj_pasek, 0, config.powtorzenia, "Badanie GA");

    for (int i = 0; i < config.powtorzenia; i++) {
        // Czyścimy plik konwergencji dla PIERWSZEGO uruchomienia (i=0)
        string plik_konw_do_przekazania = (i == 0) ? config.plik_konwergencji : "";
        if (i == 0 && !plik_konw_do_przekazania.empty()) {
            ofstream plik_konw(plik_konw_do_przekazania, ios::trunc); // Zawsze nadpisujemy stary wykres
            plik_konw << "Iteracja;Najlepszy_Koszt\n";
            plik_konw.close();
        }

        stoper.start(); 
        
        // Wywołanie rdzenia Algorytmu Genetycznego
        Osobnik wynik = Algorytm_Genetyczny::wykonaj(graf, config, plik_konw_do_przekazania);
        
        stoper.stop();
        
        double czas_ms = stoper.pobierzCzasMs();
        int koszt = wynik.koszt;
        
        suma_czasow_ms += czas_ms;
        suma_kosztow += koszt;
        if (koszt < najlepszy_znaleziony_koszt) najlepszy_znaleziony_koszt = koszt;

        pokazPostep(config.wyswietlaj_pasek, i + 1, config.powtorzenia, "Badanie GA");
    }

    double sredni_czas = suma_czasow_ms / config.powtorzenia;
    double sredni_koszt = (double)suma_kosztow / config.powtorzenia;
    double sredni_blad = (optimum > 0) ? ((sredni_koszt - optimum) / optimum) * 100.0 : -1.0;

    if (config.wyswietlaj_pasek) cout << "\n";

    cout << "Zakonczono! Najlepszy koszt z " << config.powtorzenia << " prob: " << najlepszy_znaleziony_koszt << "\n";
    if (optimum > 0) {
        cout << "Sredni czas: " << sredni_czas << " ms | Sredni blad: " << sredni_blad << " %\n";
    } else {
        cout << "Sredni czas: " << sredni_czas << " ms | Sredni blad: N/A (brak optimum w pliku)\n";
    }

    // Zapis do CSV
    plik_csv << sciezka << ";" << N << ";" << optimum << ";" << config.powtorzenia << ";" 
             << sredni_koszt << ";" << sredni_blad << ";" << sredni_czas << ";" 
             << config.ga_rozmiar_populacji << ";" << config.ga_max_pokolen << ";" 
             << config.ga_wsp_krzyzowania << ";" << config.ga_metoda_krzyzowania << ";" 
             << config.ga_wsp_mutacji << ";" << config.ga_metoda_mutacji << ";" 
             << config.ga_hybryda_2opt << "\n";
             
    plik_csv.close();
}