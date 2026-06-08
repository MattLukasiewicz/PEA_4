#pragma once
#include <vector>
#include "Osobnik.h"
#include "Macierze.h"

int oblicz_koszt_ga(const std::vector<int>& trasa, const Macierz& macierz);
void mutacja_inwersja(std::vector<int>& trasa);
std::vector<int> krzyzowanie_OX(const std::vector<int>& rodzic1, const std::vector<int>& rodzic2);
Osobnik selekcja_turniejowa(const std::vector<Osobnik>& populacja, int rozmiar_turnieju);