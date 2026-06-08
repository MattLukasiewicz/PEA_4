#pragma once
#include <vector>

struct Osobnik {
    std::vector<int> trasa;
    int koszt;

    // Przeciążenie operatora mniejszości, by łatwo sortować populację (od najlepszego do najgorszego)
    bool operator<(const Osobnik& inny) const {
        return koszt < inny.koszt;
    }
};