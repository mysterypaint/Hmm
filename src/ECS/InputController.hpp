#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class InputController : public Component {
public:
	TransformComponent *transform;
	InputListener input;
	float t = 4.0f;
	int moveRoomX = 0;
	int moveRoomY = 0;

	~InputController() {
	}

	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Step() override {
		input.PHL_ScanInput();

		moveRoomX = 0;
		moveRoomY = 0;

		transform->velocity.x = -input.btnLeft.held + input.btnRight.held;
		transform->velocity.y = -input.btnUp.held + input.btnDown.held;

		moveRoomX += 0;//-input.btnLeft.pressed + input.btnRight.pressed;
		moveRoomY += 0;//-input.btnUp.pressed + input.btnDown.pressed;

		if (input.btnFaceLeft.pressed == 1)
			t -= 0.1f;
		else if (input.btnFaceUp.pressed == 1)
			t += 0.1f;
	}
};