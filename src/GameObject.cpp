#include "GameObject.hpp"
//#include "TextureManager.h"

GameObject::GameObject(int _tex, int _x, int _y) {
	//renderer = _ren;
	objTexture = PHL_LoadTexture(_tex);//PHL_LoadSubTexture(_tex, 0, 0, 0, 0);
	x = _x;
	y = _y;
}

void GameObject::Step() {
	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;
	x++;
}

void GameObject::Draw() {
	PHL_DrawSurface(x, y, objTexture);
	//SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect); // Draw the texture
}