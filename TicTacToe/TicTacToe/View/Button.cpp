#include "Button.h"
Button::Button(int x, int y, int xSize, int ySize) {
	this->xPosition = x;
	this->yPosition = y;
	this->xSize = xSize;
	this->ySize = ySize;
	state = ButtonState::Active;
}

bool Button::isClicked(int xMouse, int yMouse) {
	if (xMouse >= xPosition and xMouse <= xPosition + xSize
		and yMouse >= yPosition and yMouse <= yPosition + ySize) {
		state = ButtonState::Inactive;
		return true;
	}

	return false;
}