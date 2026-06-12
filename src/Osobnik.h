#pragma once
#include <vector>

struct Osobnik {
    std::vector<int> trasa;
    int koszt;


    bool operator<(const Osobnik& inny) const {
        return koszt < inny.koszt;
    }
};