#pragma once
#include <string>
//#include "../PHL.hpp"
#include "Components.hpp"
#include <math.h>

class TileColliderComponent : public Component {
private:
	World* worldPtr;
	TransformComponent* transform;
	int currArea = MAP01;
	int currRoomX = 3;
	int currRoomY = 1;
	int roomWidth = 0, roomHeight = 0, tileWidth = 0, tileHeight = 0;
	int screenOffsetX = 0, screenOffsetY = 0;

	ResourceComponent *resources;
public:
	PHL_Rect collider;
	int t;

	TileColliderComponent(int _t, World* _worldPtr) {
		t = _t;
		worldPtr = _worldPtr;
	}

	void Create() override {
		// First ensure that this entity has a ResourceComponent. Give it one if it doesn't have one already.
		/*
		if(!entity->HasComponent<ResourceComponent>()) {
			entity->AddComponent<ResourceComponent>();
		}*/
		transform = &entity->GetComponent<TransformComponent>();
		resources = &entity->GetComponent<ResourceComponent>();
		//worldPtr = entity->GetComponent<ResourceComponent>().GetWorld();
		roomWidth = resources->roomWidth;
		roomHeight = resources->roomHeight;
		tileWidth = resources->tileWidth;
		tileHeight = resources->tileHeight;
		screenOffsetX = resources->screenOffsetX;
		screenOffsetY = resources->screenOffsetY;
	}

	void Step() override {
	}

	int GetTile(float _x, float _y) {
		int _tx = (int) (fmod(floor(_x / tileWidth)+roomWidth, roomWidth));
		int _ty = (int) (fmod(floor(_y / tileHeight)+roomHeight, roomHeight));
		int _thisTile = worldPtr->area[currArea].room[(currRoomX + 4) % 4][(currRoomY + 5) % 5].tileData[(_ty * roomWidth) + _tx].tileID;
		return _thisTile;
	}
};