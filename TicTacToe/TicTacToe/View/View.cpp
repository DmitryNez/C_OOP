#include <iostream>
#include "View.h"
#include <windows.h>

View::View(TicTacToe* game, Controller* controller){
    background.loadFromFile("../Spray/background.jpg");
    cross.loadFromFile("../Spray/cross.png");
    zero.loadFromFile("../Spray/zero.png");
    mainMenu.loadFromFile("../Spray/menu.png");
    afterGame.loadFromFile("../Spray/afterGame.png");

    backgroundSprite.setTexture(background);
    afterGameSprite.setTexture(afterGame);
    crossSprite.setTexture(cross);
    zeroSprite.setTexture(zero);
    mainMenuSprite.setTexture(mainMenu);

    backgroundSize = background.getSize();
    crossSize = cross.getSize();
    zeroSize = zero.getSize();

    window.create(sf::VideoMode(backgroundSize.x, backgroundSize.y), "TicTacToe");
    this->game = game;
    this->controller = controller;
    controller->setView(this);
}

std::pair<int, int> View::gamePositionToPixel(int pos) {
    switch (pos)
    {
    case 0:
        return std::make_pair(0,0);
    case 1:
        return std::make_pair(120, 0);
    case 2:
        return std::make_pair(240, 0);
    case 3:
        return std::make_pair(0, 120);
    case 4:
        return std::make_pair(120, 120);
    case 5:
        return std::make_pair(240, 120);
    case 6:
        return std::make_pair(0, 240);
    case 7:
        return std::make_pair(120, 240);
    case 8:
        return std::make_pair(240, 240);
    }
}

int View::pixelToGamePosition(int x, int y) {
    if (y <= 120) {
        return x / int(backgroundSize.x / 3) + y / int(backgroundSize.y / 3);
    }
    if (y > 120 and y <= 240) {
        return x / int(backgroundSize.x / 3) + y / int(backgroundSize.y / 3) + 2;
    }
    if (y > 240) {
        return x / int(backgroundSize.x / 3) + y / int(backgroundSize.y / 3) + 4;
    }
}

void View::drawMainMenu(){
    window.draw(mainMenuSprite);
}

void View::drawTextAndAfterGame() {
    window.draw(afterGameSprite);
    window.draw(text);
}

void View::drawField() {
    window.draw(backgroundSprite);
}

void View::pushCross(int x, int y) {
    sf::Vector2i scaledPos(int(x / crossSize.x) * crossSize.x, int(y / crossSize.y) * crossSize.y);
    crossSprite.setPosition(scaledPos.x, scaledPos.y);
    crosses.push_back(crossSprite);
}

void View::pushZero(int x, int y) {
    sf::Vector2i scaledPos(int(x / zeroSize.x) * zeroSize.x, int(y / zeroSize.y) * zeroSize.y);
    zeroSprite.setPosition(scaledPos.x, scaledPos.y);
    zeros.push_back(zeroSprite);
}

void View::clear() {
    window.clear();
}

void View::close() {
    window.close();
}

void View::drawSymbols() {
    for (auto it : crosses) {
        window.draw(it);
    }

    for (auto it : zeros) {
        window.draw(it);
    }
}

sf::RenderWindow& View::getWindow() {
    return window;
}

void View::display() {
    window.display();
}

void View::gameLoop() {
    Symbol symbol = game->whoIsNext();
    Symbol winner = Symbol::Empty;
    HumanPlayer* PlayerFirst = nullptr;
    HumanPlayer* PlayerSecond = nullptr;
    ComputerPlayer* CompPlayer = nullptr;
    bool initializeFlag = 1;
    GameMode mode;
    sf::Font font;
    font.loadFromFile("../Font/arial.ttf");

    while (window.isOpen())
    {
        winner = game->getWinner();
        while (window.pollEvent(event)) {
            if (winner != Symbol::Empty) {
                if (game->getWinner() == Symbol::X) {
                    text = sf::Text("X - winner!", font, 40);
                    text.setFillColor(sf::Color::Blue);
                    text.setStyle(sf::Text::Bold);
                    text.setPosition(85, 30);
                }
                else {
                    text = sf::Text("O - winner!", font, 40);
                    text.setFillColor(sf::Color::Red);
                    text.setStyle(sf::Text::Bold);
                    text.setPosition(85, 30);
                }

                drawTextAndAfterGame();
                display();

                againButton.state = ButtonState::Active;
                backToMenuButton.state = ButtonState::Active;

                int flag = controller->afterGameMouseClick(event);

                crosses.clear();
                zeros.clear();

                if (flag != -1) { // Again or BackToMenu button is clicked
                    winner = Symbol::Empty;
                    initializeFlag = flag;
                }
            }
            else if (game->isDrawn()) {
                text = sf::Text("Drawn!", font, 40);
                text.setFillColor(sf::Color::Black);
                text.setStyle(sf::Text::Bold);
                text.setPosition(120, 30);

                drawTextAndAfterGame();
                display();

                againButton.state = ButtonState::Active;
                backToMenuButton.state = ButtonState::Active;

                int flag = controller->afterGameMouseClick(event);

                crosses.clear();
                zeros.clear();

                if (flag != -1) { // Again or BackToMenu button is clicked
                    winner = Symbol::Empty;
                    initializeFlag = flag;
                }
            }
            else {
                if (initializeFlag == 1) {
                    drawMainMenu();
                    display();

                    mode = GameMode(controller->initializeMouseClick(event));

                    if (mode == GameMode::vsComputer) {

                        if (PlayerSecond != nullptr) {
                            delete PlayerFirst;
                            delete PlayerSecond;
                            delete CompPlayer;
                            PlayerSecond = nullptr;
                        }

                        PlayerFirst = new HumanPlayer(Symbol::O);
                        CompPlayer = new ComputerPlayer(Symbol::X);

                        event.type = sf::Event::EventType::MouseButtonReleased;
                        initializeFlag = 0;
                    }
                    else if (mode == GameMode::vsHuman) {

                        if (CompPlayer != nullptr) {
                            delete PlayerFirst;
                            delete PlayerSecond;
                            delete CompPlayer;
                            CompPlayer = nullptr;
                        }

                        PlayerFirst = new HumanPlayer(Symbol::X);
                        PlayerSecond = new HumanPlayer(Symbol::O);

                        event.type = sf::Event::EventType::MouseButtonReleased;
                        initializeFlag = 0;
                    }
                }

                if (initializeFlag == 0) {

                    if (CompPlayer == nullptr) {

                        drawField();

                        drawSymbols();

                        display();

                        controller->leftMouseFieldClick(event, PlayerFirst, PlayerSecond, symbol);

                    }
                    else {

                        drawField();

                        drawSymbols();

                        display();

                        controller->leftMouseFieldClick(event, PlayerFirst, symbol);

                        if (symbol == CompPlayer->getSymbol()) {
                            auto field = game->getField();

                            int pos = CompPlayer->computerMove(*field);

                            if (pos != -1) {
                                auto pixelPos = gamePositionToPixel(pos);

                                if (CompPlayer->getSymbol() == Symbol::O) {
                                    pushZero(pixelPos.first, pixelPos.second);
                                }
                                else if (CompPlayer->getSymbol() == Symbol::X) {
                                    pushCross(pixelPos.first, pixelPos.second);
                                }

                                symbol = game->whoIsNext();
                            }
                        }
                    }
                }
            }
        }
    }

    delete PlayerFirst;
    delete PlayerSecond;
    delete CompPlayer;
}
