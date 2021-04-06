#include "Controller.h"
Controller::Controller(TicTacToe* game) {
	this->game = game;
    this->view = nullptr;
}

void Controller::setView(View* view) {
	this->view = view;
}

void Controller::leftMouseFieldClick(sf::Event event, HumanPlayer* firstPlayer, HumanPlayer* secondPlayer, Symbol& symbol) {
    if (event.type == sf::Event::Closed) {
        view->close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.key.code == sf::Mouse::Left) {

            sf::Vector2i pixelPos = sf::Mouse::getPosition(view->getWindow());
            int pos = view->pixelToGamePosition(pixelPos.x, pixelPos.y);
            auto field = game->getField();

            if (symbol == firstPlayer->getSymbol()) {

                if (firstPlayer->humanMove(pos, *field)) {

                    if (firstPlayer->getSymbol() == Symbol::O) {
                        view->pushZero(pixelPos.x, pixelPos.y);
                    }
                    else if (firstPlayer->getSymbol() == Symbol::X) {
                        view->pushCross(pixelPos.x, pixelPos.y);
                    }
                    symbol = game->whoIsNext();
                }
            }
            else {
                if (secondPlayer->humanMove(pos, *field)) {

                    if (secondPlayer->getSymbol() == Symbol::O) {
                        view->pushZero(pixelPos.x, pixelPos.y);
                    }
                    else if (secondPlayer->getSymbol() == Symbol::X) {
                        view->pushCross(pixelPos.x, pixelPos.y);
                    }
                    symbol = game->whoIsNext();
                }
            }
            
        }
    }
}

void Controller::leftMouseFieldClick(sf::Event event, HumanPlayer* humanPlayer, Symbol& symbol) {
    if (event.type == sf::Event::Closed) {
        view->close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.key.code == sf::Mouse::Left) {

            sf::Vector2i pixelPos = sf::Mouse::getPosition(view->getWindow());
            int pos = view->pixelToGamePosition(pixelPos.x, pixelPos.y);
            auto field = game->getField();

            if (symbol == humanPlayer->getSymbol()) {

                if (humanPlayer->humanMove(pos, *field)) {

                    if (humanPlayer->getSymbol() == Symbol::O) {
                        view->pushZero(pixelPos.x, pixelPos.y);
                    }
                    else if (humanPlayer->getSymbol() == Symbol::X) {
                        view->pushCross(pixelPos.x, pixelPos.y);
                    }

                    symbol = game->whoIsNext();
                }
            }

        }
    }
}

int Controller::initializeMouseClick(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        view->close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.key.code == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(view->getWindow());
            if (view->vsComputerButton.state == ButtonState::Active) {
                if (view->vsComputerButton.isClicked(pixelPos.x, pixelPos.y)) {
                    return 0;
                }
            }
            if (view->vsHumanButton.state == ButtonState::Active) {
                if (view->vsHumanButton.isClicked(pixelPos.x, pixelPos.y)) {
                    return 1;
                }
            }
        }
    }

    return -1;
}

int Controller::afterGameMouseClick(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        view->close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.key.code == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(view->getWindow());
            if (view->againButton.state == ButtonState::Active) {
                if (view->againButton.isClicked(pixelPos.x, pixelPos.y)) {
                    game->reset();
                    return 0;
                }
            }
            if (view->backToMenuButton.state == ButtonState::Active) {
                if (view->backToMenuButton.isClicked(pixelPos.x, pixelPos.y)) {

                    view->vsComputerButton.state = ButtonState::Active;
                    view->vsHumanButton.state = ButtonState::Active;

                    game->reset();
                    return 1;
                }
            }
        }
    }

    return -1;
}
