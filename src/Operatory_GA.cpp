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

    auto wykonaj_OX = [n, p1, p2](const Osobnik& r1, const Osobnik& r2, Osobnik& d) {
        d.trasa.assign(n, -1);
        unordered_set<int> skopiowane;

        for (int i = p1; i <= p2; ++i) {
            d.trasa[i] = r1.trasa[i];
            skopiowane.insert(r1.trasa[i]);
        }

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

void Operatory_GA::krzyzowanie_PMX(const Osobnik& rodzic1, const Osobnik& rodzic2, Osobnik& dziecko1, Osobnik& dziecko2) {
    int n = rodzic1.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int punkt1 = dist(gen);
    int punkt2 = dist(gen);
    if (punkt1 > punkt2) swap(punkt1, punkt2);

    auto wykonaj_PMX = [n, punkt1, punkt2](const Osobnik& r1, const Osobnik& r2, Osobnik& d) {
        d.trasa.assign(n, -1);
        unordered_set<int> skopiowane;

        for (int i = punkt1; i <= punkt2; ++i) {
            d.trasa[i] = r1.trasa[i];
            skopiowane.insert(r1.trasa[i]);
        }

        for (int i = punkt1; i <= punkt2; ++i) {
            int element_r2 = r2.trasa[i];
            if (skopiowane.find(element_r2) == skopiowane.end()) {
                int aktualna_pozycja = i;
                int element_r1 = r1.trasa[aktualna_pozycja];
                
                // Szukamy, gdzie w r2 znajduje się element_r1
                while (true) {
                    auto it = find(r2.trasa.begin(), r2.trasa.end(), element_r1);
                    int index_w_r2 = distance(r2.trasa.begin(), it);
                    
                    if (d.trasa[index_w_r2] == -1) {
                        d.trasa[index_w_r2] = element_r2;
                        skopiowane.insert(element_r2);
                        break;
                    } else {
                        aktualna_pozycja = index_w_r2;
                        element_r1 = r1.trasa[aktualna_pozycja];
                    }
                }
            }
        }

        // 3. Wypełnij pozostałe puste luki miastami z r2
        for (int i = 0; i < n; ++i) {
            if (d.trasa[i] == -1) {
                d.trasa[i] = r2.trasa[i];
            }
        }
    };

    wykonaj_PMX(rodzic1, rodzic2, dziecko1);
    wykonaj_PMX(rodzic2, rodzic1, dziecko2);
}


void Operatory_GA::mutacja_inwersja(Osobnik& osobnik) {
    int n = osobnik.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int p1 = dist(gen);
    int p2 = dist(gen);
    if (p1 > p2) swap(p1, p2);

    reverse(osobnik.trasa.begin() + p1, osobnik.trasa.begin() + p2 + 1);
}

void Operatory_GA::mutacja_scramble(Osobnik& osobnik) {
    int n = osobnik.trasa.size();
    uniform_int_distribution<> dist(0, n - 1);
    
    int p1 = dist(gen);
    int p2 = dist(gen);
    if (p1 > p2) swap(p1, p2);

    shuffle(osobnik.trasa.begin() + p1, osobnik.trasa.begin() + p2 + 1, gen);
}