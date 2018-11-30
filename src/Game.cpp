#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include "Game.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include <math.h>
//#include "AssetManager.hpp"

////// Global objects //////
Manager manager;
auto& test(manager.AddEntity());
auto& test2(manager.AddEntity());
////// Global vars //////
int tick = 0;
double deltaTime = 1.0f;
int screenShakeTimer = 0;
bool gamePaused = false;
bool loopBGM = true;

PHL_Music currSong;					// Keeps the current song stream in the memory
PHL_Sound sounds[SE_MAX];			// Creates an array that stores every sound in the game

PHL_Background background, foreground;
PHL_Surface textures[T_MAX] = {0};

int language = ENG;
int texID = sprMapG00;
int aniTest[4] = {295, 292, 293, 294};
const float _tAniSpdMult = 4.0f; // Tile Animation Speed Multiplier

Game::Game(void) {
	Startup();
}

Game::~Game(void) {
}

void Game::Startup(void) {
	if (_gameState != INIT)
		return;

	//Setup
	PHL_Init();
	//assets->AddTexture(T_PROTAG, sprProt1);

	textures[T_PROTAG] = PHL_LoadTexture(sprProt1);
	textures[T_ATEX] = PHL_LoadTexture(sprMapG01);
	//Force create save data folders
	#ifdef _3DS
		//3DS builds
		mkdir("sdmc:/3ds", 0777);
		mkdir("sdmc:/3ds/appdata", 0777);
		mkdir("sdmc:/3ds/appdata/La-Mulana", 0777);
		mkdir("sdmc:/3ds/appdata/La-Mulana/data", 0777);
	#else
		//psp, wii
		mkdir("/data", 0777);
	#endif

	test.AddComponent<TestComponent>(3450934859);
	test.AddComponent<TestComponent2>(8);
	//test2.AddComponent<TestComponent>(4);
}

void Game::Ending(void) {
	PHL_Deinit();
}

int Game::GetMainLoop() {
	return PHL_MainLoop();
}

bool Game::IsExiting() {
	if(_gameState == Game::EXITING) 
		return true;
	else 
		return false;
}

void Game::Step() {
	manager.Refresh();
	manager.Step();


	switch(_gameState) {
		case Game::INIT:

			_gameState = Game::GAMEPLAY;
			break;
		case Game::GAMEPLAY:
			break;
		default:
			break;
	}


	if (GetMainLoop() == 0) {

	}
	tick++;
}

void Game::Draw() {
	PHL_StartDrawing();
	PHL_DrawBackground(background, foreground);



	manager.Draw();
}

Game::GameState Game::_gameState = INIT;