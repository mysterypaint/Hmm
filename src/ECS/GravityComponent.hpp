#pragma once
#include "ECS.hpp"
#include "TransformComponent.hpp"

class Gravity : public Component {
public:
	TransformComponent *transform;
	SpriteComponent *sprite;
	
	PHL_Rect tileRect;
	int tileID;
	std::string path;

	Gravity() = default;

	Gravity(int _x, int _y, int _w, int _h, int _id) {
		tileRect.x = _x;
		tileRect.y = _y;
		tileRect._w = _w;
		tileRect._h = _h;
		tileID = _id;

		path = "romfs:/graphics/";

		switch(tileID) {
			case 0:
				path += "tile0.bmp";
			case 1:
				path += "tile1.bmp";
			default:
				break;
		}
	}
}