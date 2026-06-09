#include "WczytywanieKonfiguracji.h"
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

WczytywanieKonfiguracji::WczytywanieKonfiguracji() {
    wyswietlaj_pasek = false;
    plik_wynikowy = "wyniki.csv";
    plik_konwergencji = "konwergencja.csv";
    powtorzenia = 1;
    ga_rozmiar_populacji = 100;
    ga_max_pokolen = 500;
    ga_wsp_krzyzowania = 0.8;
    ga_metoda_krzyzowania = "OX";
    ga_wsp_mutacji = 0.05;
    ga_metoda_mutacji = "INW";
    ga_elitaryzm = 1;
    ga_hybryda_2opt = false;
    sciezka_do_pliku = "";
}

bool WczytywanieKonfiguracji::wczytaj_z_pliku(const string& nazwa_pliku) {
    ifstream plik(nazwa_pliku);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku configu: " << nazwa_pliku << endl;
        return false;
    }

    string klucz;
    string znak_rownosci;

    while (plik >> klucz) {
        if (klucz.substr(0, 2) == "//") {
            string reszta_linii;
            getline(plik, reszta_linii);
            continue;
        }

        plik >> znak_rownosci;

        if (klucz == "WYSWIETLAJ_PASEK_POSTEPU") plik >> wyswietlaj_pasek;
        else if (klucz == "PLIK_WYNIKOWY") plik >> plik_wynikowy;
        else if (klucz == "PLIK_KONWERGENCJI") plik >> plik_konwergencji;
        else if (klucz == "POWTORZENIA") plik >> powtorzenia;
        else if (klucz == "GA_ROZMIAR_POPULACJI") plik >> ga_rozmiar_populacji;
        else if (klucz == "GA_MAX_POKOLEN") plik >> ga_max_pokolen;
        else if (klucz == "GA_WSP_KRZYZOWANIA") plik >> ga_wsp_krzyzowania;
        else if (klucz == "GA_METODA_KRZYZOWANIA") {
            plik >> ga_metoda_krzyzowania;
            for (char& c : ga_metoda_krzyzowania) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
        else if (klucz == "GA_WSP_MUTACJI") plik >> ga_wsp_mutacji;
        else if (klucz == "GA_METODA_MUTACJI") {
            plik >> ga_metoda_mutacji;
            for (char& c : ga_metoda_mutacji) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
        else if (klucz == "GA_ELITARYZM") plik >> ga_elitaryzm;
        else if (klucz == "GA_HYBRYDA_2OPT") plik >> ga_hybryda_2opt;
        else if (klucz == "SCIEZKA_DO_PLIKU") plik >> sciezka_do_pliku;
    }

    plik.close();
    return true;
}