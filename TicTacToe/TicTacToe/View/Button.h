#pragma once
enum class ButtonState {Active, Inactive};
class Button
{
public:
	int xPosition;
	int yPosition;
	int xSize;
	int ySize;
	ButtonState state;

	Button(int, int, int, int);
	bool isClicked(int, int);
};

