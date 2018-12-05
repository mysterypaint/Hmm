#pragma once

#include "Resources.hpp"
#include "PHL.hpp"

//class AssetManager;
class Game {
public:
	int screenShakeTimer;

	enum GameState { INIT, LOGO1, LOGO2, ATTRACT,
		TITLE, GAMEPLAY, ROOMSWITCHING, PAUSED, EXITING, GS_MAX};

	Game();
	~Game();
	static void Startup();
	static void Ending();
	static int GetMainLoop();
	static void Step();
	static void GameplayStep();
	static void Draw();
	static void UpdateMapGraphics();
	static void UpdateMapBGM();
	static void DrawRoom(Area* _thisArea, Room* _thisRoom, int _texID, int _drawXOffset, int _drawYOffset);
	//static AssetManager* assets;

private:
	static bool IsExiting();
	static GameState gameState;
};