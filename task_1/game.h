#pragma once
#include <fstream>
#include <string>
#include "cell.h"
#include "field.h"
class game
{
public:
	game();
	~game();
	void initial_distr_of_cells(std::string& file, field& initial);
	int counter_of_neighbors(const int& x, const int& y, field&);
	void next_generation(field&);
	void play(std::string& file, size_t number_of_iter, field& game_field);
};
