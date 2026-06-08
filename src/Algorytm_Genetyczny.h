#pragma once
#include "Macierze.h"
#include "Osobnik.h"

#include <cstddef>

Osobnik uruchom_GA(const Macierz& macierz, size_t rozmiar_populacji, int max_pokolen, double wsp_mutacji, double wsp_krzyzowania);