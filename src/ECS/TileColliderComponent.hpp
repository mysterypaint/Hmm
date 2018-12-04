#pragma once
#include <string>
//#include "../PHL.hpp"
#include "Components.hpp"
#include <math.h>

class TileColliderComponent : public Component {
private:
	World* worldPtr;
	TransformComponent* transform;
	int* currArea;
	int* currRoomX;
	int* currRoomY;
	int* roomWidth;
	int* roomHeight;
	int* tileWidth;
	int* tileHeight;
	ResourceComponent *resources;
public:
	PHL_Rect collider;
	int t;

	TileColliderComponent(int _t, Entity* _levelData) {
		t = _t;
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
		// First ensure that this entity has a ResourceComponent. Give it one if it doesn't have one already.
		/*
		if(!entity->HasComponent<ResourceComponent>()) {
			entity->AddComponent<ResourceComponent>();
		}*/
		transform = &entity->GetComponent<TransformComponent>();
		resources = &entity->GetComponent<ResourceComponent>();
	}

	void Step() override {
	}

	int GetTile(float _x, float _y) {
		int _tx = (int) floor(fmod((_x / *tileWidth)+ *roomWidth, *roomWidth));
		int _ty = (int) floor(fmod((_y / *tileHeight)+ *roomHeight, *roomHeight));
		int _thisTile = worldPtr->area[*currArea].room[(*currRoomX + 4) % 4][(*currRoomY + 5) % 5].tileData[(_ty * *roomWidth) + _tx].tileID;
		return _thisTile;
	}

	int TileType(int _tileID) {
		int _x = _tileID % 40;
		int _y = _tileID / 40;
		int _val = worldPtr->collisionMap[*currArea].tile[_x][_y];
		return _val;
	}

	bool TileAABB(int _x, int _y, int _w, int _h, int _tx, int _ty) {
		PHL_Rect _recA, _recB;
		_recA.x = _x;
		_recA.y = _y;
		_recA.w = _w;
		_recA.h = _h;
		_recB.x = _tx;
		_recB.y = _ty;
		_recB.w = 8; // All the tiles are 8x8
		_recB.h = 8;

		if (
			_recA.x + _recA.w >= _recB.x &&
			_recB.x + _recB.w >= _recA.x &&
			_recA.y + _recA.h >= _recB.y &&
			_recB.y + _recB.h >= _recA.y
			)
			return true; // There was a collision
		return false; // There wasn't
	}
};