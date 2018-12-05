#pragma once
#include "ECS.hpp"
#include "../Resources.hpp"

class CameraComponent : public Component {
private:
	InputListener* input;
	int moveX = 0, moveY = 0, moveSpeed = 6;
public:
	int dir = DIR_RIGHT;
	float x = 0;
	float y = 0;
	float xTo = 0;
	float yTo = 0;
	float screenOffsetX = 32;
	float screenOffsetY = 32;

	CameraComponent() {
	}

	void Create() override {
		input = &entity->GetComponent<InputController>().input;
	}

	void Step() override {
		if (input->btnR.held) {
			moveX = (-input->btnLeft.held + input->btnRight.held) * 4;
			moveY = (-input->btnUp.held + input->btnDown.held) * 4;
		} else {
			moveX = 0;
			moveY = 0;
		}

		x += moveX;
		y += moveY;

		switch(dir) {
			default:
				break;
			case DIR_LEFT:
				if (x < xTo)
					x += moveSpeed;
				else {
					x = xTo;
					dir = DIR_NONE;
				}
				break;
			case DIR_RIGHT:
				if (x > xTo)
					x -= moveSpeed;
				else {
					x = xTo;
					dir = DIR_NONE;
				}
				break;
			case DIR_UP:
				if (y < yTo)
					y += moveSpeed;
				else {
					y = yTo;
					dir = DIR_NONE;
				}
				break;
			case DIR_DOWN:
				if (y > yTo)
					y -= moveSpeed;
				else {
					y = yTo;
					dir = DIR_NONE;
				}
				break;
		}
	}
};