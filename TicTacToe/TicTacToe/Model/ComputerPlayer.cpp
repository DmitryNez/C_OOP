#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(Symbol playerSymbol) {
	this->computerSymbol = playerSymbol;
}

bool ComputerPlayer::isValidMove(size_t MoveTo, const Field& gameField) {
	auto field = gameField.getField();
	if (MoveTo < 9) {
		if (field[MoveTo] == Symbol::Empty) {
			return true;
		}
	}
	return false;
}

int ComputerPlayer::winMove(std::vector<size_t> row, const Field& gameField, Symbol playerSymbol) {
	int countOfPlayerSymbol = 0, indexOfEmpty = -1;
	auto field = gameField.getField();

	for (size_t i = 0; i < row.size(); ++i) {
		if (field[row[i]] == playerSymbol) {
			countOfPlayerSymbol++;
		}
		else if (field[row[i]] == Symbol::Empty) {
			indexOfEmpty = row[i];
		}
	}

	if (countOfPlayerSymbol == 2) {
		return indexOfEmpty;
	}

	return -1;
}

Symbol ComputerPlayer::getOpponent() {
	if (computerSymbol == Symbol::O) {
		return Symbol::X;
	}
	return Symbol::O;
}

int ComputerPlayer::computerMove(Field& field) {

	std::vector<std::vector<size_t>> winCombination = field.getWinCombination();

	/*If the win position is exist now, do it*/
	for (size_t i = 0; i < winCombination.size(); ++i) {
		int WinPosition = winMove(winCombination[i], field, computerSymbol);
		if (WinPosition != -1 && isValidMove(WinPosition, field)) {
			field.setSymbolOnPosition(computerSymbol, WinPosition);
			return WinPosition;
		}
	}

	/*If opponent wins in next step, computer does not allow it*/
	Symbol opponet = getOpponent();

	for (size_t i = 0; i < winCombination.size(); ++i) {
		int WinPositionForOpponent = winMove(winCombination[i], field, opponet);
		if (WinPositionForOpponent != -1 && isValidMove(WinPositionForOpponent, field)) {
			field.setSymbolOnPosition(computerSymbol, WinPositionForOpponent);
			return WinPositionForOpponent;
		}
	}

	/*Else to choose the most valuable move*/
	for (size_t i = 0; i < bestMove.size(); ++i) {
		if (isValidMove(bestMove[i], field)) {
			field.setSymbolOnPosition(computerSymbol, bestMove[i]);
			return bestMove[i];
		}
	}

	return -1;
}

Symbol ComputerPlayer::getSymbol() {
	return computerSymbol;
}