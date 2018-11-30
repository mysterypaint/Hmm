#pragma once
#include "ECS.hpp"
#include "../Resources.hpp"

class ResourceComponent : public Component {
public:
	World* worldPtr;
	int roomWidth = 32;
	int roomHeight = 22;
	int tileWidth = 8;
	int tileHeight = 8;
	int screenOffsetX = 32;
	int screenOffsetY = 32;

	ResourceComponent() = default;

	ResourceComponent(World* _worldPtr) {
		World* worldPtr = _worldPtr;
		printf("Player entity (Resource): %x\n", &entity);
		printf("ResourceComponent arg0: %x\n", _worldPtr);
		printf("ResourceComponent: %x\n", worldPtr);
	}

	World* GetWorld() {
		return worldPtr;
	}

	void Create() override {
		//int _thisTile = worldPtr->area[1].room[3 % 4][(0 + 5) % 5].tileData[(5 * roomWidth) + 5].tileID;
		//printf("%d\n", _thisTile);
	}
};