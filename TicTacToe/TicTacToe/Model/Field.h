#pragma once
#pragma once
#include <vector>
#include "Symbol.h"

class Field {
private:
	std::vector<Symbol> field;

	std::vector<std::vector<size_t>> winCombination{ {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6} };
public:
	Field();

	std::vector<Symbol> getField() const;

	void setSymbolOnPosition(Symbol, size_t);

	std::vector<std::vector<size_t>> getWinCombination();

	void clear();
};