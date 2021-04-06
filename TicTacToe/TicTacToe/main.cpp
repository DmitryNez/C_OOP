#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "View/View.h"
#include "Controller/Controller.h"

using namespace std;

int main()
{
    Field* field = new Field();

    TicTacToe* newGame = new TicTacToe(Symbol::O, Symbol::X, field);

    Controller* controller = new Controller(newGame);

    View view(newGame, controller);

    view.gameLoop();

    delete field;
    delete newGame;
    delete controller;
}
