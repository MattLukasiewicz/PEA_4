#include "Operatory_GA.h"
#include <algorithm>
#include <cstdlib>

using namespace std;

int oblicz_koszt_ga(const vector<int>& trasa, const Macierz& macierz) {
    int suma = 0;
    int n = trasa.size();
    for (int i = 0; i < n - 1; ++i) {
        suma += macierz.dane[trasa[i]][trasa[i + 1]];
    }
    suma += macierz.dane[trasa[n - 1]][trasa[0]]; // Powrót do startu
    return suma;
}

void mutacja_inwersja(vector<int>& trasa) {
    int n = trasa.size();
    int start = rand() % n;
    int koniec = rand() % n;
    if (start > koniec) swap(start, koniec);
    
    reverse(trasa.begin() + start, trasa.begin() + koniec + 1);
}

vector<int> krzyzowanie_OX(const vector<int>& rodzic1, const vector<int>& rodzic2) {
    int n = rodzic1.size();
    vector<int> dziecko(n, -1);
    
    // Losowanie fragmentu
    int start = rand() % n;
    int koniec = rand() % n;
    if (start > koniec) swap(start, koniec);

    // 1. Skopiuj fragment z rodzica 1
    for (int i = start; i <= koniec; ++i) {
        dziecko[i] = rodzic1[i];
    }

    // 2. Wypełnij puste miejsca z rodzica 2
    int idx_dziecko = (koniec + 1) % n;
    int idx_rodzic2 = (koniec + 1) % n;

    for (int i = 0; i < n; ++i) {
        int miasto = rodzic2[idx_rodzic2];
        
        // Sprawdź czy to miasto już jest u dziecka
        bool istnieje = false;
        for (int j = start; j <= koniec; ++j) {
            if (dziecko[j] == miasto) {
                istnieje = true;
                break;
            }
        }

        // Jeśli nie ma, wstaw
        if (!istnieje) {
            dziecko[idx_dziecko] = miasto;
            idx_dziecko = (idx_dziecko + 1) % n;
        }
        idx_rodzic2 = (idx_rodzic2 + 1) % n;
    }

    return dziecko;
}

Osobnik selekcja_turniejowa(const vector<Osobnik>& populacja, int rozmiar_turnieju) {
    int n = populacja.size();
    Osobnik najlepszy = populacja[rand() % n];

    for (int i = 1; i < rozmiar_turnieju; ++i) {
        Osobnik kandydat = populacja[rand() % n];
        if (kandydat.koszt < najlepszy.koszt) {
            najlepszy = kandydat;
        }
    }
    return najlepszy;
}