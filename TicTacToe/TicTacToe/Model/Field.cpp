#include <string>
#include "Field.h"

Field::Field() {
	for (int i = 0; i < 9; ++i) {
		field.push_back(Symbol::Empty);
	}
}

std::vector<Symbol> Field::getField() const {
	return field;
}

void Field::setSymbolOnPosition(Symbol symbol, size_t position) {
	if (position > 8) {
		std::string error_message = "Position ";
		error_message = error_message + std::to_string(position) + " does not exist";
		throw std::exception(error_message.c_str());
	}
	else {
		field[position] = symbol;
	}
}

std::vector<std::vector<size_t>> Field::getWinCombination() {
	return winCombination;
}

void Field::clear() {
	for (int i = 0; i < field.size(); ++i) {
		field[i] = Symbol::Empty;
	}
}