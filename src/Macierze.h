#pragma once
#include <vector>

class Macierz {
public:
	int n;
	std::vector<std::vector<int>> dane;
	int optymalnyKoszt;
	static const int INF = 1000000000;

	Macierz(int size = 0) : n(size), dane(size, std::vector<int>(size, INF)), optymalnyKoszt(-1) {
		for (int i = 0; i < n; ++i) dane[i][i] = INF;
	}

	int at(int i, int j) const { return dane[i][j]; }
};

