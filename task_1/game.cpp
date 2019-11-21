#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <sfml/Graphics.hpp>
#include <ctime>
#include "game.h"

game::game() {

}
game::~game() {

}
void game::initial_distr_of_cells(std::string& file, field& initial) {
	std::ifstream fin(file);
	std::string s;
	int j = -1;
	while (std::getline(fin, s)) {
		for (int i = 0; i != s.size(); ++i) {
			if (j == -1) {
				std::stringstream str(s);
				std::vector<size_t> tmp(2);
				str >> tmp[0];
				str >> tmp[1];
				size_t size_x = tmp[0];
				size_t size_y = tmp[1];
				initial.set_size(size_x, size_y);
				break;
			}
			if (s[i] == 42) {
				state s = state::alive;
				initial.field_of_cells[j][i].set_status(s);
			}
			if (s[i] == 95) {
				state s = state::empty;
				initial.field_of_cells[j][i].set_status(s);
			}
		}
		j++;
	}

}
int game::counter_of_neighbors(const int& x, const int& y, field& life) {
	int counter = 0;
	size_t size_x = life.get_size_x();
	size_t size_y = life.get_size_y();
	for (int dx = -1; dx < 2; dx++) {
		for (int dy = -1; dy < 2; dy++) {
			int nX = x + dx;
			int nY = y + dy;

			if ((nX < 0 || nY < 0) || (nX > size_x - 1 || nY > size_y - 1)) {
				break;
			}
			else
				counter += (life.field_of_cells[nX][nY].get_status() == state::alive) ? 1 : 0;
		}
	}
	if (life.field_of_cells[x][y].get_status() == state::alive) { counter--; } // если клетка живая, то при dx = 0 и dy = 0 она будет засчитана
	return counter;
}
 void game::next_generation(field& life) {

	size_t size_x = life.get_size_x();
	size_t size_y = life.get_size_y();
	field next_gen;
	next_gen.set_size(size_x, size_y);
	state s;
	
	for (size_t i = 0; i < size_x; ++i) {
		for (size_t j = 0; j < size_y; ++j) {

			int count = counter_of_neighbors(i, j, life);
			next_gen.field_of_cells[i][j] = life.field_of_cells[i][j];

			if (next_gen.field_of_cells[i][j].get_status() == state::empty) {
				s = (count == 3) ? state::alive : next_gen.field_of_cells[i][j].get_status();
				next_gen.field_of_cells[i][j].set_status(s);
				continue;
			}
			else {
				s = ((count < 2) || (count > 3)) ? state::empty : next_gen.field_of_cells[i][j].get_status();
				next_gen.field_of_cells[i][j].set_status(s);
			}
		}
	}
	
	std::swap(next_gen.field_of_cells, life.field_of_cells);
	next_gen.field_of_cells.erase(next_gen.field_of_cells.begin(), next_gen.field_of_cells.end());
	next_gen.field_of_cells.shrink_to_fit();

}

 std::ostream& operator <<(std::ostream& stream, field& game_field) {

	 size_t size_x = game_field.get_size_x();
	 size_t size_y = game_field.get_size_y();
	 for (size_t i = 0; i < size_x; i++) {
		 for (size_t j = 0; j < size_y; j++) {
			 if (game_field.field_of_cells[i][j].get_status() == state::alive) {
				 stream << "*";
			 }
			 stream << "_";
		 }
		 stream << std::endl;
	 }
	 return stream;
 }
 //void game::play(std::string& file, size_t number_of_iter, field& game_field) {

	// initial_distr_of_cells(file, game_field);
	// size_t size_x = game_field.get_size_x();
	// size_t size_y = game_field.get_size_y();
	//
	// for (size_t i = 0; i < number_of_iter; i++) {
	//	 next_generation(game_field);
	// }
	// std::cout << game_field;

 //}

void game::play(std::string& file, size_t number_of_iter, field& game_field) {
	_CrtMemState s1;
	_CrtMemState s2;
	_CrtMemState s3;

	_CrtMemCheckpoint(&s1);
	initial_distr_of_cells(file, game_field);
	size_t size_x = game_field.get_size_x();
	size_t size_y = game_field.get_size_y();

	int k = 0;
	sf::RenderWindow window(sf::VideoMode(size_y * 8, size_x * 8), "Life");
	//sf::RenderWindow window(sf::VideoMode(940, 640), "Life");
	sf::RectangleShape rectangle;

	sf::Texture back;
 	back.loadFromFile("Background1.png");

	sf::Sprite background(back);


	rectangle.setSize(sf::Vector2f(7, 7));
	rectangle.setFillColor(sf::Color::Green);

	window.setFramerateLimit(60);

	_CrtMemCheckpoint(&s2);

	if (_CrtMemDifference(&s3, &s1, &s2))
		_CrtMemDumpStatistics(&s3);
	_CrtDumpMemoryLeaks();
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

	while (window.isOpen()) {
		
	sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		
			if (event.type == sf::Event::KeyPressed && number_of_iter == 0) {
				if ((event.key.code == sf::Keyboard::Space)) {
					 next_generation(game_field);
					k = 0;
				}
			}
		}

		
 		if (number_of_iter != 0) {
			
			for (size_t i = 0; i < number_of_iter; i++) {
			 next_generation(game_field);
			}

		}
		
		if (k == 0) {
			
			window.draw(background);
			
			for (size_t i = 0; i < size_x; i++) {
				for (size_t j = 0; j < size_y; j++) {
					if (game_field.field_of_cells[i][j].get_status() == state::alive) {
						rectangle.setPosition((j + 2. / 8) * 8 , (i + 2. / 8) * 8);
						window.draw(rectangle);
					}
				}
			}
			
			window.display();
			window.clear();
			
		}
		k++;
		
		
	}

}

