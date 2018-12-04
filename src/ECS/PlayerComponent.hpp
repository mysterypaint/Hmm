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
	SpriteTexture* sTex;
	World* worldPtr;
	ResourceComponent* resources;

	int sprOriginX, sprOriginY;

	int* ticks;
	float* deltaTime;
	bool* gamePaused;
	int* currArea;
	int* currRoomX;
	int* currRoomY;
	int* roomWidth;
	int* roomHeight;
	int* tileWidth;
	int* tileHeight;
	float* x;
	float* y;

	// Player Physics
	int state = P_IDLE;
	bool grounded = false;
	bool hasBoots = false;
	float moveSpeed = 0.6;
	float grav = 0.1;
	float grav_max = 3;
	float jumpSpeed = 3;

public:
	float hsp = 0;
	float vsp = 0;
	int facingX = DIR_RIGHT;
	int facingY = DIR_DOWN;
	float bboxLeft;
	float bboxRight;
	float bboxTop;
	float bboxBottom;
	PHL_Rect collider;

	PlayerComponent(Entity* _levelData) {
		LevelData* levelData = &_levelData->GetComponent<LevelData>();
		worldPtr = levelData->GetWorld();
		currArea = &levelData->currArea;
		currRoomX = &levelData->currRoomX;
		currRoomY = &levelData->currRoomY;
		roomWidth = &levelData->roomWidth;
		roomHeight = &levelData->roomHeight;
		tileWidth = &levelData->tileWidth;
		tileHeight = &levelData->tileHeight;
	}

	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();
		tCollider = &entity->GetComponent<TileColliderComponent>();
		input = &entity->GetComponent<InputController>().input;
		time = &entity->GetComponent<TimeComponent>();
		resources = &entity->GetComponent<ResourceComponent>();
		sTex = &entity->GetComponent<SpriteComponent>().sTex;
		sprOriginX = sTex->originX;
		sprOriginY = sTex->originY;

		ticks = time->ticks;
		deltaTime = time->deltaTime;
		gamePaused = time->gamePaused;

		x = &transform->position.x;
		y = &transform->position.y;

		if (!hasBoots)
			moveSpeed = 0.6;
		else
			moveSpeed = 0.8;
	}

	void Step() override {
		bboxLeft = *x - sTex->originX;
		bboxRight = *x + sTex->w - sTex->originX - 1;
		bboxTop = *y - sTex->originY;
		bboxBottom = *y + sTex->h - sTex->originY - 1;

		if (*deltaTime > 0)
			PlayerMovementCode();
		//printf("deltaTime: %f       \nPlayer: (%f, %f)              \nPlayer on Tile: %d               \nOrigin: (%d, %d)\033[0;0H", *deltaTime, *x, *y, _thisTile, sprOriginX, sprOriginY);
	}


	void PlayerMovementCode() {
		//facingX
		int moveX = 0;
		int moveY = 0;
		if (!input->btnR.held) {
			moveX = (-input->btnLeft.held + input->btnRight.held);
			moveY = (-input->btnUp.held + input->btnDown.held);
		}
		int tSize = 8;
		if (moveX != 0) { // Are we facing right or left? (Right initially)
			if (moveX > 0)
				facingX = DIR_RIGHT;
			else
				facingX = DIR_LEFT;
		} else
			facingX = DIR_NONE;

	/*
		Decompose movement into X and Y axes, step one at a time. If you’re planning on implementing slopes afterwards, step X first, then Y. Otherwise, the order shouldn’t matter much. Then, for each axis:
    
	    *Get the coordinate of the forward-facing edge, e.g. : If walking left, the x coordinate of left of bounding box. If walking right, x coordinate of right side. If up, y coordinate of top, etc.
	    
	    Figure which lines of tiles the bounding box intersects with – this will give you a minimum and maximum tile value on the OPPOSITE axis. For example, if we’re walking left, perhaps
	    the player intersects with horizontal rows 32, 33 and 34 (that is, tiles with y = 32 * TS, y = 33 * TS, and y = 34 * TS, where TS = tile size).
	    
	    Scan along those lines of tiles and towards the direction of movement until you find the closest static obstacle.
	    Then loop through every moving obstacle, and determine which is the closest obstacle that is actually on your path.
	    
	    The total movement of the player along that direction is then the minimum between the distance to closest obstacle, and the amount that you wanted to move in the first place.
	    
	    Move player to the new position. With this new position, step the other coordinate, if still not done.
	*/
		float start;
		float end;
		float ts;
		float tsF;
		bool breakLoop = false;

		hsp = moveX * 2;

		if (facingX == DIR_LEFT) {
			start = floor(bboxTop / tSize);
			end = ceil(bboxBottom / tSize);
			ts = floor((bboxLeft) / tSize);
			tsF = floor(((bboxLeft) + hsp) / tSize) - 1;
			for (int tx = ts; !breakLoop && tx != tsF; tx--) {
				for (int ty = start; ty != end; ty++) {
			        int tileID = worldPtr->area[*currArea].room[(*currRoomX + 4) % 4][(*currRoomY + 5) % 5].tileData[(ty * *roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = bboxLeft - ((tx+1) * tSize);
			    		hsp = -min(abs(hsp), distance);
			    		breakLoop = true;
			    		break;
			    	}
			    }
			}
		} else if (facingX == DIR_RIGHT) {
			start = floor(bboxTop / tSize);
			end = ceil(bboxBottom / tSize);
			ts = floor((bboxRight + 1) / tSize);
			tsF = floor(((bboxRight + 1) + hsp) / tSize) + 1;
			for (int tx = ts; !breakLoop && tx != tsF; tx++) {
				for (int ty = start; ty != end; ty++) {
			        int tileID = worldPtr->area[*currArea].room[(*currRoomX + 4) % 4][(*currRoomY + 5) % 5].tileData[(ty * *roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = ((tx) * tSize) - (bboxRight + 1);
			    		hsp = min(abs(hsp), distance);
			    		breakLoop = true;
			    		break;
			    	}
			    }
			}
		}

		//vsp = moveY * 2;
		if (input->btnFaceDown.pressed && grounded) {
			vsp = -jumpSpeed;
			PHL_PlaySound(resources->sounds[SE03], 1);
		}

		if (vsp >= 0)
			facingY = DIR_DOWN;
		else {
			grounded = false;
			facingY = DIR_UP;
		}


		breakLoop = false;
		start = floor(bboxLeft / tSize);      // bounds = Rectangle of your entity
		end = ceil(bboxRight / tSize);
		ts = floor(bboxTop / tSize);
		tsF = floor((bboxTop + vsp) / tSize) - 1;
		bool onCeiling  = false;
		if (facingY == DIR_UP) {
			for (int ty = ts; !breakLoop && ty != tsF; ty--) {
				for (int tx = start; tx != end; tx++) {
			        int tileID = worldPtr->area[*currArea].room[(*currRoomX + 4) % 4][(*currRoomY + 5) % 5].tileData[(ty * *roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = bboxTop - ((ty+1) * tSize);
			    		vsp = -min(abs(vsp), distance);
			    		if (distance == 0 && *deltaTime > 0) {
			    			*y = ((ty+1) * tSize) + sTex->h;
			    			onCeiling  = true;
			    		}
			    		breakLoop = true;
			    		break;
			    	}
			    }
			}
		} else if (facingY == DIR_DOWN) {
			start = floor(bboxLeft / tSize);
			end = ceil(bboxRight / tSize);
			ts = floor((bboxBottom+1) / tSize);
			tsF = floor(((bboxBottom+1) + vsp) / tSize) + 1;
			for (int ty = ts; !breakLoop && ty != tsF; ty++) {
				for (int tx = start; tx != end; tx++) {
			        int tileID = worldPtr->area[*currArea].room[(*currRoomX + 4) % 4][(*currRoomY + 5) % 5].tileData[(ty * *roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = ((ty) * tSize) - (bboxBottom+1);
			    		vsp = min(abs(vsp), distance);
			    		breakLoop = true;
			    		if (!grounded) {
			    			PHL_PlaySound(resources->sounds[SE04], 1);
			    		}
						grounded = true;
			    		break;
			    	}
			    }
			}
		}
		if (vsp < grav_max)
			vsp += grav * *deltaTime;

		//printf("%f, %f          \n\033[0;0H",hsp,vsp);

		//int _thisTile = tCollider->GetTile(*x, *y);

		//tCollider->TileType(_thisTile)

		//printf("Position: (%f, %f)\n\nbboxLeft: %f\nbboxRight: %f\nbboxTop: %f\nbboxBottom: %f\033[0;0H", *x, *y, bboxLeft, bboxRight, bboxTop, bboxBottom);

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

		*x += hsp * *deltaTime;
		if (!onCeiling)
			*y += vsp * *deltaTime;
	}

	void Collide(int axis) {

	}

	void Draw() override {
//void PHL_DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, PHL_RGB col) {
/*
		PHL_RGB color = PHL_NewRGB(255,0,0);
		if (facingX == DIR_LEFT)
			PHL_DrawRect(bboxLeft, *y - sTex->originY, 1, 16, color);
		else
			PHL_DrawRect(bboxRight, *y - sTex->originY, 1, 16, color);

		if (facingY == DIR_DOWN)
			PHL_DrawRect(*x - sTex->originX, bboxBottom, 16, 1, color);
		else
			PHL_DrawRect(*x - sTex->originX, bboxTop, 16, 1, color);
*/
	}
};