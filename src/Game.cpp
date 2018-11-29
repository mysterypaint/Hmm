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
//AssetManager* Game::assets = new AssetManager(&manager);
auto& wall(manager.addEntity());
auto& player(manager.addEntity());
auto& levelData(manager.addEntity());

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
	textures[T_ATEX] = PHL_LoadTexture(sprMapG00);
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

	player.AddComponent<TransformComponent>(10);
	//player.AddComponent<SpriteComponent>(textures[T_PROTAG]);
	player.AddComponent<InputController>();
	player.AddComponent<ColliderComponent>("player");

	wall.AddComponent<TransformComponent>(370.0f, 377.0f, 8, 8, 1);
	//wall.AddComponent<SpriteComponent>(textures[T_PROTAG]);
	wall.AddComponent<ColliderComponent>("wall");

	levelData.AddComponent<LevelData>(language);
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

	if (Collision::AABB(player.GetComponent<ColliderComponent>().collider,
		wall.GetComponent<ColliderComponent>().collider)) {
		//player.GetComponent<TransformComponent>().scale = 1;
		player.GetComponent<TransformComponent>().velocity * -1;
		// Wall hit
	}

	switch(_gameState) {
		case Game::INIT:


			PHL_FreeMusic(currSong);
			currSong = PHL_LoadMusic("m01", loopBGM);
			//currSong = PHL_LoadMusic(NULL, loopBGM);
			sounds[SE00] = PHL_LoadSound("se00");
			sounds[SE01] = PHL_LoadSound("se01");
			PHL_PlayMusic(currSong);
			//textures[T_ATEX] = PHL_LoadTexture(levelData.GetComponent<LevelData>().world.texID[MAP00]);
			//player.GetComponent<LevelData>().LoadMap(0, textures[T_ATEX]);
			_gameState = Game::GAMEPLAY;
			break;
		case Game::GAMEPLAY:
			break;
		default:
			break;
	}


	/*
	if (input.btnStart.pressed == 1) {
		gamePaused = !gamePaused;
		if (gamePaused) {
			deltaTime = 0;
			PHL_StopMusic();
			PHL_PlaySound(sounds[Game::SE00], 1);
			//PHL_StopSound(sounds[se00], 1); // PHL_Sound snd, int channel
		} else {
			deltaTime = 1;
			PHL_PlayMusic(currSong);
			PHL_PlaySound(sounds[Game::SE00], 1);
		}
	}

	if (input.btnSelect.pressed == 1)
		PHL_GameQuit();
	*/

	if (GetMainLoop() == 0) {
		// The game is ending; Free all game resources

		//Free sounds
		for (int i = 0; i < SE_MAX; i++) {
			PHL_FreeSound(sounds[i]);
		}
		
		//Free Music
		PHL_FreeMusic(currSong);
	}
	tick++;
}

void Game::Draw() {
	PHL_StartDrawing();
	//PHL_DrawBackground(background, foreground);





	//PHL_DrawSurface(0, 0, textures[T_ATEX]);

	//map[i].room[0][0].tileData[0].tileID = 5;
	//PHL_DrawSurfacePart(32, 32, 16, 0, 16, 16, textures[T_PROTAG]);



//void PHL_DrawSurfacePart(int16_t x, int16_t y, int16_t cropx, int16_t cropy, int16_t cropw, int16_t croph, PHL_Surface surf) {
	if (player.GetComponent<InputController>().input.btnFaceRight.pressed == 1) {
		levelData.GetComponent<LevelData>().LoadMap(1, textures[T_ATEX]);
		PHL_FreeSurface(textures[T_ATEX]);
		texID = levelData.GetComponent<LevelData>().world.texID[levelData.GetComponent<LevelData>().currArea];
		textures[T_ATEX] = PHL_LoadTexture(texID);

		switch(levelData.GetComponent<LevelData>().currArea) {
			case MAP00:
				PHL_FreeMusic(currSong);
				currSong = PHL_LoadMusic("m01", loopBGM);
				PHL_PlayMusic(currSong);
				break;
			case MAP01:
				PHL_FreeMusic(currSong);
				currSong = PHL_LoadMusic("m00", loopBGM);
				PHL_PlayMusic(currSong);
				break;
		}
	} else if (player.GetComponent<InputController>().input.btnFaceDown.pressed == 1) {
		levelData.GetComponent<LevelData>().LoadMap(-1, textures[T_ATEX]);
		PHL_FreeSurface(textures[T_ATEX]);
		texID = levelData.GetComponent<LevelData>().world.texID[levelData.GetComponent<LevelData>().currArea];
		textures[T_ATEX] = PHL_LoadTexture(texID);
		switch(levelData.GetComponent<LevelData>().currArea) {
			case MAP00:
				PHL_FreeMusic(currSong);
				currSong = PHL_LoadMusic("m01", loopBGM);
				PHL_PlayMusic(currSong);
				break;
			case MAP01:
				PHL_FreeMusic(currSong);
				currSong = PHL_LoadMusic("m00", loopBGM);
				PHL_PlayMusic(currSong);
				break;
		}
	}

/*
	printf("                      \033[0;0H");
	printf("Map: ");
	if (levelData.GetComponent<LevelData>().currArea < 10)
		printf("0");
	printf("%d", levelData.GetComponent<LevelData>().currArea);
	if (levelData.GetComponent<LevelData>().currArea >= BOSS00)
		printf(" (Boss Room)");

*/

	// Get the current room coordinates we're playing in

	int currRoomX = levelData.GetComponent<LevelData>().currRoomX;
	int currRoomY = levelData.GetComponent<LevelData>().currRoomY;

	// Debug room switching controls
	int moveRoomX = player.GetComponent<InputController>().moveRoomX;
	int moveRoomY = player.GetComponent<InputController>().moveRoomY;

	// Find the current room, given the room coordinates
	Room* currRoom = &levelData.GetComponent<LevelData>().world.area[levelData.GetComponent<LevelData>().currArea].room[currRoomX % 4][currRoomY % 5];

	// Switch room data based on the cardinal directions we've pressed (debug)
	if (moveRoomX == -1) { // Left
		if (currRoom->mapLeft.x >= 0 && currRoom->mapLeft.y >= 0) {	
			levelData.GetComponent<LevelData>().currArea = currRoom->mapLeft.area;
			levelData.GetComponent<LevelData>().currRoomX = currRoom->mapLeft.x;
			levelData.GetComponent<LevelData>().currRoomY = currRoom->mapLeft.y;
			currRoomX = levelData.GetComponent<LevelData>().currRoomX;
			currRoomY = levelData.GetComponent<LevelData>().currRoomY;
			UpdateMapGraphics();
			UpdateMapBGM();
		}
	} else if (moveRoomX == 1) { // Right
		if (currRoom->mapRight.x >= 0 && currRoom->mapRight.y >= 0) {
			levelData.GetComponent<LevelData>().currArea = currRoom->mapRight.area;
			levelData.GetComponent<LevelData>().currRoomX = currRoom->mapRight.x;
			levelData.GetComponent<LevelData>().currRoomY = currRoom->mapRight.y;
			currRoomX = levelData.GetComponent<LevelData>().currRoomX;
			currRoomY = levelData.GetComponent<LevelData>().currRoomY;
			UpdateMapGraphics();
			UpdateMapBGM();
		}
	}

	if (moveRoomY == -1) { // Up
		if (currRoom->mapUp.x >= 0 && currRoom->mapUp.y >= 0) {
			levelData.GetComponent<LevelData>().currArea = currRoom->mapUp.area;
			levelData.GetComponent<LevelData>().currRoomX = currRoom->mapUp.x;
			levelData.GetComponent<LevelData>().currRoomY = currRoom->mapUp.y;
			currRoomX = levelData.GetComponent<LevelData>().currRoomX;
			currRoomY = levelData.GetComponent<LevelData>().currRoomY;
			UpdateMapGraphics();
			UpdateMapBGM();
		}
	} else if (moveRoomY == 1) { // Down
		if (currRoom->mapDown.x >= 0 && currRoom->mapDown.y >= 0) {
			levelData.GetComponent<LevelData>().currArea = currRoom->mapDown.area;
			levelData.GetComponent<LevelData>().currRoomX = currRoom->mapDown.x;
			levelData.GetComponent<LevelData>().currRoomY = currRoom->mapDown.y;
			currRoomX = levelData.GetComponent<LevelData>().currRoomX;
			currRoomY = levelData.GetComponent<LevelData>().currRoomY;
			UpdateMapGraphics();
			UpdateMapBGM();
		}
	}

	Area* _thisArea = &levelData.GetComponent<LevelData>().world.area[levelData.GetComponent<LevelData>().currArea];
	Room* _thisRoom = &_thisArea->room[currRoomX % 4][(currRoomY + 5) % 5];
	int roomID = _thisRoom->roomGroupID;
	int l1 = _thisRoom->mapLeft.area;
	int l2 = _thisRoom->mapLeft.x;
	int l3 = _thisRoom->mapLeft.y;
	int r1 = _thisRoom->mapRight.area;
	int r2 = _thisRoom->mapRight.x;
	int r3 = _thisRoom->mapRight.y;
	int u1 = _thisRoom->mapUp.area;
	int u2 = _thisRoom->mapUp.x;
	int u3 = _thisRoom->mapUp.y;
	int d1 = _thisRoom->mapDown.area;
	int d2 = _thisRoom->mapDown.x;
	int d3 = _thisRoom->mapDown.y;

	//printf("\nanitest: %d\nRoom: (%d, %d)\nRoom Group ID: %d                     \ntexID: %d                     \nUp %d,%d,%d   \nRight %d,%d,%d   \nDown %d,%d,%d   \nLeft: %d,%d,%d   \nUDLR - Move around the map\nA - Go forward a map\nB - Go back a map\033[0;0H", aniTest[0], currRoomX % 4, currRoomY % 5, roomID, texID - 4, u1,u2,u3, r1,r2,r3, d1,d2,d3, l1,l2,l3);


/*
	for (int _y = 0; _y < 22; _y++) {
		for (int _x = 0; _x < 32; _x++) {
			int _thisTile = levelData.GetComponent<LevelData>().world.area[levelData.GetComponent<LevelData>().currArea].room[currRoomX % 4][(currRoomY-1 + 5) % 5].tileData[(_y * 32) + _x].tileID;
			PHL_DrawSurfacePart((_x * 8) + 32, (_y * 8) + 32 - (8 * 22),
								(_thisTile % 40) * 8, (_thisTile/40) * 8,
								8, 8, textures[T_ATEX]);
		}
	}
*/
	

	// Loop through every single Room[_x][_y] tile to draw every single tile in a given room
	for (int _y = 0; _y < 22; _y++) { 
		for (int _x = 0; _x < 32; _x++) {
			Tile* _thisTile = &_thisRoom->tileData[(_y * 32) + _x];
			int _thisTileIndex = _thisTile->tileID;

			if (_thisTileIndex >= 1160) { // Determine if the tile is animated or not. The last row of tiles are always animated

				// Determine how long each frame should be in the animation
				float _aniWait = _thisArea->tileAnimationList[_thisTile->tileID-1160].waitFrames;

				// Determine how many frames the animation is. Account for the first frame (+1), which is the base tile we're working with
				int _aniMax = _thisArea->tileAnimationList[_thisTile->tileID-1160].aniTiles.size() + 1;

				// Increment the animation frame if we've waited long enough
				if (tick % (int)(_aniWait * _tAniSpdMult) == 0) // Increment the animation frame every <wait> * <tile animation speed multiplier> ticks
					_thisTile->aniIndex++;
				
				// Ensure that the current tile never goes over the maximum animation value
				_thisTile->aniIndex = fmod(_thisTile->aniIndex, _aniMax);
				int _thisAniIndex = floor(_thisTile->aniIndex);

				vector<int>* _aniTiles = &_thisArea->tileAnimationList[_thisTile->tileID-1160].aniTiles; // Get the current tile's tile animation list

				if (_thisAniIndex < 1) {
					PHL_DrawSurfacePart((_x * 8) + 32, (_y * 8) + 32, (_thisTileIndex % 40) * 8, (_thisTileIndex/40) * 8, 8, 8, textures[T_ATEX]); // Draw the first frame normally
					//printf("%d\n", _thisTileIndex);
				} else {
					PHL_DrawSurfacePart((_x * 8) + 32, (_y * 8) + 32, (_aniTiles->at(_thisAniIndex - 1) % 40) * 8, (_aniTiles->at(_thisAniIndex - 1)/40) * 8, 8, 8, textures[T_ATEX]); // For all the other frames, load the index dynamically
				}
			} else {
				PHL_DrawSurfacePart((_x * 8) + 32, (_y * 8) + 32, (_thisTileIndex % 40) * 8, (_thisTileIndex/40) * 8, 8, 8, textures[T_ATEX]);
			}
		}
	}

/*

	float __x = player.GetComponent<TransformComponent>().position.x;
	float __y = player.GetComponent<TransformComponent>().position.y;

	printf("X:%f\nY:%f\033[0;0H", __x, __y);

*/
	//PHL_DrawSurfacePart(32, 32, 16, 0, 16, 16, textures[T_PROTAG]); //PHL_DrawSurfacePart(int16_t x, int16_t y, int16_t cropx, int16_t cropy, int16_t cropw, int16_t croph, PHL_Surface surf) 

	manager.Draw();
	PHL_EndDrawing();
}

void Game::UpdateMapGraphics() {
	PHL_FreeSurface(textures[T_ATEX]);
	texID = levelData.GetComponent<LevelData>().world.texID[levelData.GetComponent<LevelData>().currArea];
	textures[T_ATEX] = PHL_LoadTexture(texID);
}

void Game::UpdateMapBGM() {

}




Game::GameState Game::_gameState = INIT;