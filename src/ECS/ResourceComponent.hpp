#pragma once
#include "ECS.hpp"
#include "../Resources.hpp"

class ResourceComponent : public Component {
public:
	PHL_Surface* textures;
	PHL_Sound* sounds;
	PHL_Music* currBGM;

	int roomWidth = 32;
	int roomHeight = 22;
	int tileWidth = 8;
	int tileHeight = 8;
	int screenOffsetX = 32;
	int screenOffsetY = 32;

	ResourceComponent(PHL_Surface* _textures, int texArraySize, PHL_Sound* _sounds, int soundsArraySize, PHL_Music* _currBGM, int bgmArraySize) {
		textures = _textures;
		sounds = _sounds;
		currBGM = _currBGM;
	}

	void Create() override {
		//int _thisTile = worldPtr->area[1].room[3 % 4][(0 + 5) % 5].tileData[(5 * roomWidth) + 5].tileID;
		//printf("%d\n", _thisTile);
	}
};