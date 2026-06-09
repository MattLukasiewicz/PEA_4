#pragma once
#include <vector>
#include <string>
#include "Osobnik.h"
#include "Macierze.h"

class Operatory_GA {
public:
    // Selekcja
    static int selekcja_turniejowa(const std::vector<Osobnik>& populacja, int rozmiar_turnieju);

    // Krzyżowanie
    static void krzyzowanie_OX(const Osobnik& rodzic1, const Osobnik& rodzic2, Osobnik& dziecko1, Osobnik& dziecko2);
    static void krzyzowanie_PMX(const Osobnik& rodzic1, const Osobnik& rodzic2, Osobnik& dziecko1, Osobnik& dziecko2);

    // Mutacja
    static void mutacja_inwersja(Osobnik& osobnik);
    static void mutacja_scramble(Osobnik& osobnik);

    // Pomocnicze
    static int oblicz_koszt(const std::vector<int>& trasa, const std::vector<std::vector<int>>& graf);
};