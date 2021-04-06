#pragma once
#include <SFML/Graphics.hpp>
#include "../Controller/Controller.h"
#include "Button.h"
#include "../Model/Symbol.h"

class ComputerPlayer;
class HumanPlayer;
class Controller;
class TicTacToe;

class View
{
private:
	sf::Texture background, cross, zero, mainMenu, afterGame;
	sf::Sprite backgroundSprite, crossSprite, zeroSprite, mainMenuSprite, afterGameSprite;

	std::vector<sf::Sprite> crosses;
	std::vector<sf::Sprite> zeros;

	sf::Vector2u backgroundSize, crossSize, zeroSize;

	sf::Event event;

	sf::Text text;

	TicTacToe* game;
	Controller* controller;

	std::pair<int,int> gamePositionToPixel(int);
public:

	Button vsComputerButton{ 67, 123, 163, 68 }, vsHumanButton{ 67, 223, 163, 68 }, againButton{ 105, 178, 161, 56 }, backToMenuButton{ 74, 93, 216, 67 };

	sf::RenderWindow window;

	View(TicTacToe*, Controller*);

	int pixelToGamePosition(int, int);

	sf::RenderWindow& getWindow();

	void drawMainMenu();

	void drawField();

	void drawTextAndAfterGame();

	void drawSymbols();

	void pushCross(int, int);

	void pushZero(int, int);

	void gameLoop();

	void display();

	void clear();

	void close();
};

