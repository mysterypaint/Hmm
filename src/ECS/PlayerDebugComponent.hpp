#pragma once
//#include "../PHL.hpp"
#include "Components.hpp"
#include <math.h>

class PlayerDebugComponent : public Component {
private:
	InputListener* input;
	TimeComponent* time;
	TransformComponent* transform;
	int* ticks;
	float* deltaTime;
	bool* gamePaused;
	float* x;
	float* y;
	touchPosition touch;

public:
	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();
		input = &entity->GetComponent<InputController>().input;
		time = &entity->GetComponent<TimeComponent>();

		ticks = time->ticks;
		deltaTime = time->deltaTime;
		gamePaused = time->gamePaused;

		x = &transform->position.x;
		y = &transform->position.y;
	}

	void Step() override {
		//Read the touch screen coordinates
		hidTouchRead(&touch);

		if (touch.px != 0.0f && touch.py != 0.0f) {
			*x = touch.px;
			*y = touch.py;
		}
	}
};