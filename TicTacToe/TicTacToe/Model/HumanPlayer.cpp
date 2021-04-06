#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(Symbol symbol) {
	humanSymbol = symbol;
}

bool HumanPlayer::isValidMove(size_t MoveTo, const Field& gameField) {
	auto field = gameField.getField();
	if (MoveTo < 9) {
		if (field[MoveTo] == Symbol::Empty) {
			return true;
		}
	}
	return false;
}

bool HumanPlayer::humanMove(size_t MoveTo, Field& field) {
	if (isValidMove(MoveTo, field)) {
		field.setSymbolOnPosition(humanSymbol, MoveTo);
		return true;
	}
	return false;
}

Symbol HumanPlayer::getSymbol() {
	return humanSymbol;
}