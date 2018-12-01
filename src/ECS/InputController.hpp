#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class InputController : public Component {
public:
	InputListener input;

	~InputController() {
	}

	void Create() override {
	}

	void Step() override {
		input.PHL_ScanInput();
	}
};