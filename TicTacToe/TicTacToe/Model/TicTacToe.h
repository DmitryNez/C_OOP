#pragma once
#include "ComputerPlayer.h"
#include "HumanPlayer.h"

enum class GameMode {vsComputer, vsHuman};

class TicTacToe {
private:

	Field* gameField;

	Symbol firstPlayer;
	Symbol secondPlayer;

	Symbol whoIsFirst();

	Symbol previousMovePlayerNumber;

public:
	TicTacToe();

	TicTacToe(Symbol,Symbol,Field*);

	Symbol whoIsNext();

	Symbol getWinner();

	bool isDrawn();

	Field* getField() const;

	void reset();
};