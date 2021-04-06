#pragma once
#include "../View/View.h"
#include "../Model/TicTacToe.h"

class View;

class Controller
{
private:
	TicTacToe* game;
	View* view;

public:
	Controller(TicTacToe*);

	void setView(View*);

	void leftMouseFieldClick(sf::Event, HumanPlayer*, HumanPlayer*, Symbol&);

	void leftMouseFieldClick(sf::Event, HumanPlayer*, Symbol&);

	int	initializeMouseClick(sf::Event);

	int afterGameMouseClick(sf::Event);
};

