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
	int sprOriginX, sprOriginY;

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
	float bboxLeft;
	float bboxRight;
	float bboxTop;
	float bboxBottom;

	// Player Physics
	int state = P_IDLE;
	bool hasBoots = false;
	float moveSpeed = 0.6;
	float hsp = 0;
	float vsp = 0;
	float grav = 0;//0.2;
	float grav_max = 3;
	int facingX = DIR_RIGHT;
	int facingY = DIR_DOWN;

public:
	PHL_Rect collider;

	PlayerComponent(World* _worldPtr) {
		worldPtr = _worldPtr;
	}

	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();
		tCollider = &entity->GetComponent<TileColliderComponent>();
		input = &entity->GetComponent<InputController>().input;
		time = &entity->GetComponent<TimeComponent>();
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

		PlayerMovementCode();
		//printf("deltaTime: %f       \nPlayer: (%f, %f)              \nPlayer on Tile: %d               \nOrigin: (%d, %d)\033[0;0H", *deltaTime, *x, *y, _thisTile, sprOriginX, sprOriginY);
	}

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

	void PlayerMovementCode() {
		//facingX
		int moveX = (-input->btnLeft.held + input->btnRight.held);
		int moveY = (-input->btnUp.held + input->btnDown.held);
		int tSize = 8;

		if (moveX != 0) { // Are we facing right or left? (Right initially)
			if (moveX > 0)
				facingX = DIR_RIGHT;
			else
				facingX = DIR_LEFT;
		}



		// bool TileAABB(int _x, int _y, int _w, int _h, int _tx, int _ty) {

		hsp = moveX * moveSpeed;
		//hsp = moveX * moveSpeed;

/*

		if (vsp < grav_max)
			vsp += grav;


*/


		int leftTile = (bboxLeft + hsp) / tSize;      // bounds = Rectangle of your entity
		int topTile = bboxTop / tSize;
		int rightTile = (int) ceil((bboxRight + hsp) / tSize) - 1;
		int bottomTile = (int) ceil(bboxBottom / tSize) - 1;

		if (moveX <= 0) {
			for (int ty = topTile; ty <= bottomTile; ++ty) {
			    for (int tx = leftTile; tx <= rightTile; ++tx) {
			        int tileID = worldPtr->area[currArea].room[(currRoomX + 4) % 4][(currRoomY + 5) % 5].tileData[(ty * roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = abs(*x - (tx * tSize));
			    		hsp = min(hsp, distance);
			    	}
			    }
			}
		} else {
			for (int ty = bottomTile; ty >= topTile; --ty) {
			    for (int tx = rightTile; tx >= leftTile; --tx) {
			        int tileID = worldPtr->area[currArea].room[(currRoomX + 4) % 4][(currRoomY + 5) % 5].tileData[(ty * roomWidth) + tx].tileID;
			        int tileType = tCollider->TileType(tileID);
			    	if (tileType == TL_SOLID) {
			    		float distance = abs((tx * tSize) - *x);
			    		hsp = min(hsp, distance);
			    	}
			    }
			}
		}



		leftTile = bboxLeft / tSize;      // bounds = Rectangle of your entity
		topTile = (bboxTop + vsp) / tSize;
		rightTile = (int) ceil(bboxRight / tSize) - 1;
		bottomTile = (int) ceil((bboxBottom + vsp) / tSize) - 1;

		if (vsp < grav_max)
			vsp += grav;

		for (int ty = topTile; ty <= bottomTile; ++ty)
		{
		    for (int tx = leftTile; tx <= rightTile; ++tx)
		    {
		        int tileID = worldPtr->area[currArea].room[(currRoomX + 4) % 4][(currRoomY + 5) % 5].tileData[(ty * roomWidth) + tx].tileID;
		        int tileType = tCollider->TileType(tileID);
		    	if (tileType == TL_SOLID) {
		    		float distance = (ty * tSize) - *y;
		    		vsp = min(vsp, abs(distance));
		    	}
		    }
		}

		printf("%f, %f          \n\033[0;0H",hsp,vsp);

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

		*x += hsp;
		*y += vsp;
	}

	void Collide(int axis) {

	}

	void Draw() override {
//void PHL_DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, PHL_RGB col) {
		PHL_RGB color = PHL_NewRGB(255,0,0);
		if (facingX == DIR_LEFT)
			PHL_DrawRect(bboxLeft, *y - sTex->originY, 1, 16, color);
		else
			PHL_DrawRect(bboxRight, *y - sTex->originY, 1, 16, color);

		if (facingY == DIR_DOWN)
			PHL_DrawRect(*x - sTex->originX, bboxBottom, 16, 1, color);
		else
			PHL_DrawRect(*x - sTex->originX, bboxTop, 16, 1, color);
	}
};