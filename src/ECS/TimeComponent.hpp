#pragma once
//#include "../PHL.hpp"
#include "Components.hpp"
#include <math.h>

class TimeComponent : public Component {
private:
	InputListener* input;
	ResourceComponent* resources;
public:
	int* ticks;
	float* deltaTime;
	bool* gamePaused;

	TimeComponent(int* _ticks, float* _deltaTime, bool* _gamePaused) {
		ticks = _ticks;
		deltaTime = _deltaTime;
		gamePaused = _gamePaused;
	}

	void Create() override {
		input = &entity->GetComponent<InputController>().input;
		resources = &entity->GetComponent<ResourceComponent>();
	}

	void Step() override {
		if (input->btnStart.pressed == 1) {
			*gamePaused = !*gamePaused;
			if (*gamePaused) {
				*deltaTime = 0;
				PHL_StopMusic();
				PHL_StopSound(resources->sounds[SE00], 1); // PHL_Sound snd, int channel
				PHL_PlaySound(resources->sounds[SE00], 1);
			} else {
				*deltaTime = 1;
				PHL_PlayMusic(resources->currBGM[0]);
				PHL_StopSound(resources->sounds[SE00], 1); // PHL_Sound snd, int channel
				PHL_PlaySound(resources->sounds[SE00], 1);
			}
		}

		if (input->btnSelect.pressed == 1)
			PHL_GameQuit();
	}
};