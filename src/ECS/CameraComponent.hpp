#pragma once
#include "ECS.hpp"
#include "../Resources.hpp"

class CameraComponent : public Component {
private:
	InputListener* input;
	int moveX = 0, moveY = 0;
public:
	float x = 0;
	float y = 0;

	CameraComponent() {
	}

	void Create() override {
		input = &entity->GetComponent<InputController>().input;
	}

	void Step() override {
		if (input->btnR.held) {
			moveX = (-input->btnLeft.held + input->btnRight.held);
			moveY = (-input->btnUp.held + input->btnDown.held);
		} else {
			moveX = 0;
			moveY = 0;
		}

		x += moveX;
		y += moveY;
	}
};