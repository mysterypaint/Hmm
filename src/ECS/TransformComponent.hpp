#pragma once

#include "Components.hpp"
#include "../Vector2D.hpp"

// Could be a struct, where everything is public
class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent() { // If we don't provide an x and y position, put this object at (0, 0)
		position.Zero();
	}

	TransformComponent(int _sc) {
		position.Zero();
		scale = _sc;
	}

	TransformComponent(float _x, float _y) {
		position.x = _x;
		position.y = _y;
	}

	TransformComponent(float _x, float _y, int _w, int _h, int _sc) {
		position.x = _x;
		position.y = _y;
		width = _w;
		height = _h;
		scale = _sc;
	}

	void Create() override {
	}
	
	void Step() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};