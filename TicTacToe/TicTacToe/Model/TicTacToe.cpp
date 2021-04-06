#include <ctime>
#include "TicTacToe.h"


Symbol TicTacToe::whoIsFirst() {
	srand(time(0));
	return Symbol(rand() % 2);
}

TicTacToe::TicTacToe() {
	this->gameField = nullptr;
	previousMovePlayerNumber = whoIsFirst();
}

TicTacToe::TicTacToe(Symbol firstSymbol, Symbol secondSymbol, Field* field) {
	this->firstPlayer = firstSymbol;
	this->secondPlayer = secondSymbol;
	this->gameField = field;
	previousMovePlayerNumber = whoIsFirst();
}

Symbol TicTacToe::whoIsNext(){
	previousMovePlayerNumber = previousMovePlayerNumber == Symbol::O ? Symbol::X : Symbol::O;

	return previousMovePlayerNumber;
}

Symbol TicTacToe::getWinner() {
	std::vector<std::vector<size_t>> winCombination = gameField->getWinCombination();
	auto field = gameField->getField();

	for (int i = 0; i < winCombination.size(); ++i) {
		int countX = 0, countO = 0;

		for (int j = 0; j < winCombination[i].size(); ++j) {
			if (field[winCombination[i][j]] == Symbol::X) {
				countX++;
			}
			else if (field[winCombination[i][j]] == Symbol::O) {
				countO++;
			}
		}

		if (countX == 3) {
			if (firstPlayer == Symbol::X) {
				return firstPlayer;
			}
			return secondPlayer;
		}
		else if (countO == 3) {
			if (firstPlayer == Symbol::O) {
				return firstPlayer;
			}
			return secondPlayer;
		}
	}

	return Symbol::Empty;
}

bool TicTacToe::isDrawn() {
	Symbol winner = getWinner();
	size_t countEmpty = 0;
	auto field = gameField->getField();

	for (int i = 0; i < field.size(); ++i) {
		if (field[i] == Symbol::Empty) {
			countEmpty++;
		}
	}

	if (countEmpty == 0 && winner == Symbol::Empty) {
		return true;
	}

	return false;
}

Field* TicTacToe::getField() const{
	return gameField;
}

void TicTacToe::reset() {
	gameField->clear();
}