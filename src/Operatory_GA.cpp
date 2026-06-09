#include "Operatory_GA.h"
#include <random>
#include <algorithm>
#include <unordered_set>

using namespace std;

static random_device rd;
static mt19937 gen(rd());

int Operatory_GA::oblicz_koszt(const vector<int>& trasa, const vector<vector<int>>& graf) {
    int suma = 0;
    int n = trasa.size();
    if (n == 0) return 0;
    
    for (int i = 0; i < n - 1; ++i) {
        suma += graf[trasa[i]][trasa[i + 1]];
    }
    suma += graf[trasa[n - 1]][trasa[0]];
    return suma;
}

int Operatory_GA::selekcja_turniejowa(const vector<Osobnik>& populacja, int rozmiar_turnieju) {
    uniform_int_distribution<> dist(0, populacja.size() - 1);
    int najlepszy_idx = dist(gen);

    for (int i = 1; i < rozmiar_turnieju; ++i) {
        int idx = dist(gen);
        if (populacja[idx].koszt < populacja[najlepszy_idx].koszt) {
            najlepszy_idx = idx;
        }
    }
    return najlepszy_idx;
}

void Operatory_GA::krzyzowanie_OX(const Osobnik& rodzic1, const Osobnik& rodzic2, Osobnik& dziecko1, Osobnik& dziecko2) {
    int n = rodzic1.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int p1 = dist(gen);
    int p2 = dist(gen);
    if (p1 > p2) swap(p1, p2);

    // Funkcja lambda wykonująca kopiowanie OX dla pojedynczego dziecka
    auto wykonaj_OX = [n, p1, p2](const Osobnik& r1, const Osobnik& r2, Osobnik& d) {
        d.trasa.assign(n, -1);
        unordered_set<int> skopiowane;

        // 1. Kopiuj środek z pierwszego rodzica
        for (int i = p1; i <= p2; ++i) {
            d.trasa[i] = r1.trasa[i];
            skopiowane.insert(r1.trasa[i]);
        }

        // 2. Wypełniaj resztę miastami z drugiego rodzica
        int idx_dziecka = (p2 + 1) % n;
        int idx_rodzica2 = (p2 + 1) % n;

        while (idx_dziecka != p1) {
            int miasto = r2.trasa[idx_rodzica2];
            if (skopiowane.find(miasto) == skopiowane.end()) {
                d.trasa[idx_dziecka] = miasto;
                idx_dziecka = (idx_dziecka + 1) % n;
            }
            idx_rodzica2 = (idx_rodzica2 + 1) % n;
        }
    };

    wykonaj_OX(rodzic1, rodzic2, dziecko1);
    wykonaj_OX(rodzic2, rodzic1, dziecko2);
}

void Operatory_GA::mutacja_inwersja(Osobnik& osobnik) {
    int n = osobnik.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int p1 = dist(gen);
    int p2 = dist(gen);
    if (p1 > p2) swap(p1, p2);

    // Odwrócenie fragmentu wektora - niszczy tylko 2 krawędzie
    reverse(osobnik.trasa.begin() + p1, osobnik.trasa.begin() + p2 + 1);
}

void Operatory_GA::mutacja_scramble(Osobnik& osobnik) {
    int n = osobnik.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int p1 = dist(gen);
    int p2 = dist(gen);
    if (p1 > p2) swap(p1, p2);

    // Losowe przemieszanie fragmentu
    shuffle(osobnik.trasa.begin() + p1, osobnik.trasa.begin() + p2 + 1, gen);
}