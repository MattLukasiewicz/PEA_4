#ifndef WCZYTYWANIEKONFIGURACJI_H
#define WCZYTYWANIEKONFIGURACJI_H

#include <string>

class WczytywanieKonfiguracji {
public:
    bool wyswietlaj_pasek;
    std::string plik_wynikowy;
    std::string plik_konwergencji;
    int powtorzenia;

    int ga_rozmiar_populacji;
    int ga_max_pokolen;
    double ga_wsp_krzyzowania;
    std::string ga_metoda_krzyzowania;
    double ga_wsp_mutacji;
    std::string ga_metoda_mutacji;
    int ga_elitaryzm;
    int ga_rozmiar_turnieju;

    std::string sciezka_do_pliku;

    WczytywanieKonfiguracji();
    bool wczytaj_z_pliku(const std::string& nazwa_pliku);
};

#endif