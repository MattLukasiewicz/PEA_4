#pragma once
#include <vector>
#include <string>
#include "Osobnik.h"
#include "WczytywanieKonfiguracji.h"

class Algorytm_Genetyczny {
public:
    static Osobnik wykonaj(const std::vector<std::vector<int>>& graf, 
                           const WczytywanieKonfiguracji& config, 
                           const std::string& plik_konwergencji);
};