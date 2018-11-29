#pragma once

#include "Game.hpp"
#include "PHL.hpp"

class GameObject {
public:
	GameObject(int tex, int _x, int _y);
	~GameObject();

	void Step();
	void Draw();

private:
	int x;
	int y;

	PHL_Surface objTexture;
	PHL_Rect srcRect;
};