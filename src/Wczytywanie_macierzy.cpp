#include "Wczytywanie_macierzy.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <map>

using namespace std;

const map<string, int> SLOWNIK_OPTIMA = {
    {"br17", 39}, 
    {"ftv33", 1286}, 
    {"ftv35", 1473}, 
    {"ftv38", 1530}, 
    {"p43", 5620},
    {"ftv47", 1776}, 
    {"ry48p", 14422}, 
    {"ftv64", 1839},
    {"rbg323", 1326},
    {"rbg403", 2465},
    {"rbg443", 2720},
    {"gr17", 2085},
    {"gr21", 2707}, 
    {"gr24", 1272}, 
    {"bays29", 2020},
    {"dantzig42", 699},
    {"brazil58", 25395},
    {"gr48", 5046},
    {"kroA100", 21282},
    {"ch150", 6528},
    {"gr202", 40160},
    {"gr666", 294358}
};

Macierz Wczytywanie_Macierzy::wczytajMacierz(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Blad: Nie mozna otworzyc pliku struktury danych: " << nazwaPliku << "\n";
        return Macierz(0);
    }

    int n = 0;
    string linia;
    string format_wag = "FULL_MATRIX"; 
    bool format_prosty_liczbowy = false;

    while (getline(plik, linia)) {
        if (linia.find("DIMENSION") != string::npos) {
            size_t dwukropek = linia.find(':');
            if (dwukropek != string::npos) {
                n = stoi(linia.substr(dwukropek + 1));
            } else {
                stringstream ss(linia);
                string do_pominiecia;
                ss >> do_pominiecia >> n;
            }
        } else if (linia.find("EDGE_WEIGHT_FORMAT") != string::npos) {
            if (linia.find("UPPER_ROW") != string::npos) {
                format_wag = "UPPER_ROW";
            } else if (linia.find("LOWER_DIAG_ROW") != string::npos) {
                format_wag = "LOWER_DIAG_ROW";
            } else if (linia.find("UPPER_DIAG_ROW") != string::npos) {
                format_wag = "UPPER_DIAG_ROW";
            }
        } else if (linia.find("EDGE_WEIGHT_SECTION") != string::npos) {
            break; // Przerwij pętlę, poniżej są już tylko liczby
        } else if (!linia.empty() && isdigit(linia[0]) && linia.find("NAME") == string::npos) {
            stringstream ss(linia);
            ss >> n;
            format_prosty_liczbowy = true;
            break;
        }
    }

    if (n == 0) return Macierz(0);
    Macierz mat(n);

    if (format_wag == "FULL_MATRIX" || format_prosty_liczbowy) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int waga;
                plik >> waga;

                if (i == j || waga < 0 || waga == 9999 || waga >= 100000000) {
                    mat.dane[i][j] = Macierz::INF;
                } else {
                    mat.dane[i][j] = waga;
                }
            }
        }
    } else if (format_wag == "UPPER_ROW") {

        for (int i = 0; i < n; ++i) mat.dane[i][i] = Macierz::INF; 
        
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int waga;
                plik >> waga;
                mat.dane[i][j] = waga;
                mat.dane[j][i] = waga;
            }
        }
    } else if (format_wag == "UPPER_DIAG_ROW") {
        int total = n * (n + 1) / 2;
        std::vector<int> vals; vals.reserve(total);
        int v;
        while ((int)vals.size() < total && (plik >> v)) vals.push_back(v);
        if ((int)vals.size() < total) {
            plik.clear(); string tok;
            while ((int)vals.size() < total && (plik >> tok)) {
                try { vals.push_back(stoi(tok)); } catch(...) { continue; }
            }
        }
        int idx = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                int waga = (idx < (int)vals.size()) ? vals[idx++] : 0;
                if (i == j || waga < 0 || waga == 9999 || waga >= 100000000) mat.dane[i][j] = Macierz::INF;
                else mat.dane[i][j] = waga;
                mat.dane[j][i] = mat.dane[i][j];
            }
        }
    } else if (format_wag == "LOWER_DIAG_ROW") {
        int total = n * (n + 1) / 2;
        std::vector<int> vals; vals.reserve(total);
        int v;
        while ((int)vals.size() < total && (plik >> v)) {
            vals.push_back(v);
        }
        if ((int)vals.size() < total) {
            plik.clear();
            string tok;
            while ((int)vals.size() < total && (plik >> tok)) {
                try {
                    int vv = stoi(tok);
                    vals.push_back(vv);
                } catch (...) {
                    continue;
                }
            }
        }
        int idx = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= i; ++j) {
                int waga = (idx < (int)vals.size()) ? vals[idx++] : 0;
                if (i == j || waga < 0 || waga == 9999 || waga >= 100000000) {
                    mat.dane[i][j] = Macierz::INF;
                } else {
                    mat.dane[i][j] = waga;
                }
                mat.dane[j][i] = mat.dane[i][j];
            }
        }
    }

    // 3. Odczyt optimum (najpierw szukamy sum_min, potem patrzymy do słownika)
    plik.clear();
    plik.seekg(0, ios::beg);
    string token;
    bool pobrano_z_pliku = false;

    while (plik >> token) {
        size_t pozycja = token.find("sum_min=");
        if (pozycja != string::npos) {
            mat.optymalnyKoszt = stoi(token.substr(pozycja + 8));
            pobrano_z_pliku = true;
            break;
        }
    }

    if (!pobrano_z_pliku) {
        for (auto const& [klucz_pliku, wartosc_optimum] : SLOWNIK_OPTIMA) {
            if (nazwaPliku.find(klucz_pliku) != string::npos) {
                mat.optymalnyKoszt = wartosc_optimum;
                break;
            }
        }
    }

    return mat;
}