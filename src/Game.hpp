#pragma once

#include "Resources.hpp"
#include "PHL.hpp"

//class AssetManager;
class Game {
public:
	int screenShakeTimer;

	enum GameState { INIT, LOGO1, LOGO2, ATTRACT,
		TITLE, GAMEPLAY, SWITCHINGROOMS, PAUSED, EXITING, GS_MAX};

	Game();
	~Game();
	static void Startup();
	static void Ending();
	static int GetMainLoop();
	static void Step();
	static void Update();
	static void Draw();
	static void UpdateMapGraphics();
	static void UpdateMapBGM();
	//static AssetManager* assets;

private:
	static bool IsExiting();
	static GameState _gameState;
};