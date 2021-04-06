#pragma once
#include "Symbol.h"
#include "Field.h"

class HumanPlayer {
private:
	Symbol humanSymbol;
	bool isValidMove(size_t, const Field&);
public:
	HumanPlayer(Symbol);

	bool humanMove(size_t, Field&);

	Symbol getSymbol();
};