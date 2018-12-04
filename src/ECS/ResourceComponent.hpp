#pragma once
#include "ECS.hpp"
#include "../Resources.hpp"

class ResourceComponent : public Component {
public:
	PHL_Surface* textures;
	PHL_Sound* sounds;
	PHL_Music* currBGM;

	ResourceComponent(PHL_Surface* _textures, int texArraySize, PHL_Sound* _sounds, int soundsArraySize, PHL_Music* _currBGM, int bgmArraySize) {
		textures = _textures;
		sounds = _sounds;
		currBGM = _currBGM;
	}

	void Create() override {
	}
};