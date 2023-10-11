#pragma once

#include <utility>

class Input 
{
public:

	static bool isMouseButtonPressed(int mouseButton);
	static bool isKeyPressed(int keyCode);
	static bool isKeyReleased(int keyCode);
	 
	static float GetMousePositionX();
	static float GetMousePositionY();
	static std::pair<float, float> GetMousePosition();

};