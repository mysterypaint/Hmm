#pragma once
//#include "../PHL.hpp"
#include "Components.hpp"
#include <math.h>

enum Facing {
	F_LEFT,
	F_RIGHT
};

enum pStates {
	P_IDLE,
	P_MOVING,
	P_JUMPING,
	P_FALLING,
	P_WHIPPING
};

class PlayerComponent : public Component {
private:
	InputListener* input;
	TimeComponent* time;
	TransformComponent* transform;
	TileColliderComponent* tCollider;
	int* ticks;
	float* deltaTime;
	bool* gamePaused;
	int currArea = MAP01;
	int currRoomX = 3;
	int currRoomY = 1;
	int roomWidth = 32;
	int roomHeight = 22;
	int tileWidth = 8;
	int tileHeight = 8;
	int screenOffsetX = 32;
	int screenOffsetY = 32;
	float* x;
	float* y;

	// Player Physics
	int state = P_IDLE;

public:
	PHL_Rect collider;

	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();
		tCollider = &entity->GetComponent<TileColliderComponent>();
		input = &entity->GetComponent<InputController>().input;
		time = &entity->GetComponent<TimeComponent>();

		ticks = time->ticks;
		deltaTime = time->deltaTime;
		gamePaused = time->gamePaused;

		x = &transform->position.x;
		y = &transform->position.y;
/*
		if (!has_boots)
			move_speed = 0.6;
		else
			move_speed = 0.8;
*/

	}

	void Step() override {
		PlayerMovementCode();
		
		int _thisTile = tCollider->GetTile(*x, *y);

		if (*deltaTime == 1)
			printf("Ticks: %d\n", *ticks);
		//printf("Player on tile: %d\n", _thisTile);
	}

	void PlayerMovementCode() {
		switch(state) {
			default:
			case P_IDLE:
				break;
			case P_MOVING:
				break;
			case P_JUMPING:
				break;
			case P_FALLING:
				break;
			case P_WHIPPING:
				break;
		}
	}
};