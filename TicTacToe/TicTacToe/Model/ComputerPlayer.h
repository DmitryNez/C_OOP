#pragma once
#include "Symbol.h"
#include "Field.h"

class ComputerPlayer{
private:
	Symbol computerSymbol;

	int winMove(std::vector<size_t>, const Field&, Symbol);

	Symbol getOpponent(); 

	bool isValidMove(size_t, const Field&);

	std::vector<size_t> bestMove{ 4, 0, 2, 6, 8, 1, 3, 5, 7 };

public:
	ComputerPlayer(Symbol);

	int computerMove(Field&);

	Symbol getSymbol();
};