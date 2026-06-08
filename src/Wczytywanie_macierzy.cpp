#include "Wczytywanie_macierzy.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

Macierz Wczytywanie_Macierzy::wczytajMacierz(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku z macierza: " << nazwaPliku << "\n";
        return Macierz(0);
    }

    string line;
    int n = 0;
    string format = "FULL_MATRIX"; // Domyślny format
    bool formatTSPLIB = false;

    // --- FAZA 1: Przeszukiwanie nagłówka TSPLIB ---
    while (getline(plik, line)) {
        // Ignorowanie pustych linii
        if (line.empty()) continue;

        if (line.find("DIMENSION") != string::npos) {
            formatTSPLIB = true;
            size_t dwukropek = line.find(':');
            if (dwukropek != string::npos) {
                n = stoi(line.substr(dwukropek + 1));
            } else {
                // Jeśli nie ma dwukropka, szukamy pierwszej cyfry
                size_t pos = line.find_first_of("0123456789");
                if (pos != string::npos) n = stoi(line.substr(pos));
            }
        }
        else if (line.find("EDGE_WEIGHT_FORMAT") != string::npos) {
            if (line.find("UPPER_ROW") != string::npos) format = "UPPER_ROW";
            else if (line.find("LOWER_ROW") != string::npos) format = "LOWER_ROW";
            else if (line.find("UPPER_DIAG_ROW") != string::npos) format = "UPPER_DIAG_ROW";
            else if (line.find("LOWER_DIAG_ROW") != string::npos) format = "LOWER_DIAG_ROW";
            else format = "FULL_MATRIX";
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            break; // Znaleźliśmy początek danych, przerywamy pętlę i zaczynamy wczytywać liczby
        }
    }

    // Jeśli nie znaleziono słowa DIMENSION, to jest to Twój "stary" format (np. brazil58_prosty.txt)
    // Zwijamy plik do początku i wczytujemy rozmiar normalnie.
    if (!formatTSPLIB) {
        plik.clear();
        plik.seekg(0, ios::beg);
        plik >> n;
    }

    Macierz mat(n);

    // --- FAZA 2: Wczytywanie danych z uwzględnieniem formatu TSPLIB ---
    if (!formatTSPLIB || format == "FULL_MATRIX") {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int wartosc;
                plik >> wartosc;
                if (i == j || wartosc < 0) mat.dane[i][j] = INF;
                else mat.dane[i][j] = wartosc;
            }
        }
    }
    else if (format == "UPPER_ROW") {
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int wartosc;
                plik >> wartosc;
                mat.dane[i][j] = wartosc;
                mat.dane[j][i] = wartosc; // TSP symetryczny - odbijamy lustrzanie
            }
        }
    }
    else if (format == "LOWER_ROW") {
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                int wartosc;
                plik >> wartosc;
                mat.dane[i][j] = wartosc;
                mat.dane[j][i] = wartosc;
            }
        }
    }
    else if (format == "UPPER_DIAG_ROW") {
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                int wartosc;
                plik >> wartosc;
                mat.dane[i][j] = wartosc;
                mat.dane[j][i] = wartosc;
            }
        }
    }
    else if (format == "LOWER_DIAG_ROW") {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= i; ++j) {
                int wartosc;
                plik >> wartosc;
                mat.dane[i][j] = wartosc;
                mat.dane[j][i] = wartosc;
            }
        }
    }

    // W przypadku formatów uciętych TSPLIB czasami podaje "0" na przekątnej.
    // Upewniamy się, że cała przekątna to nasze zdefiniowane "INF", by algorytm nie szedł "z miasta do siebie samego".
    if (formatTSPLIB && format != "FULL_MATRIX") {
        for(int i = 0; i < n; ++i) {
            mat.dane[i][i] = INF;
        }
    }

    // --- FAZA 3: Szukanie rozwiązania optymalnego ---
    // TSPLIB kończy się czasami znacznikiem "EOF", ale wspieramy też Twoje "sum_min="
    string temp;
    while (plik >> temp) {
        if (temp == "EOF") continue;
        size_t poz = temp.find("sum_min=");
        if (poz != string::npos) {
            mat.optymalnyKoszt = stoi(temp.substr(poz + 8));
            break;
        }
    }

    return mat;
}