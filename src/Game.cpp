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
//auto& wall(manager.addEntity());
auto& player(manager.AddEntity());
auto& levelData(manager.AddEntity());
auto& camera(manager.AddEntity());

////// Global vars //////
int tick = 0;
float deltaTime = 1.0f;
int screenShakeTimer = 0;
bool gamePaused = false;
bool loopBGM = true;
InputListener* input;
PHL_Music currBGM[1];					// Keeps the current song stream in the memory
PHL_Sound sounds[SE_MAX];			// Creates an array that stores every sound in the game

PHL_Background background, foreground;
PHL_Surface textures[T_MAX] = {0};

int language = ENG;
int texID = sprMapG03;
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
	textures[T_ATEX] = PHL_LoadTexture(texID);

	PHL_FreeMusic(currBGM[0]);
	currBGM[0] = PHL_LoadMusic("m04", loopBGM);
	//currBGM[0] = PHL_LoadMusic(NULL, loopBGM);
	sounds[SE00] = PHL_LoadSound("se00");
	sounds[SE01] = PHL_LoadSound("se01");
	sounds[SE02] = PHL_LoadSound("se02");
	sounds[SE03] = PHL_LoadSound("se03");
	sounds[SE04] = PHL_LoadSound("se04");
	PHL_PlayMusic(currBGM[0]);
	//textures[T_ATEX] = PHL_LoadTexture(levelData.GetComponent<LevelData>().world.texID[MAP00]);
	//player.GetComponent<LevelData>().LoadMap(0, textures[T_ATEX]);

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

	camera.AddComponent<InputController>();
	camera.AddComponent<CameraComponent>();

	levelData.AddComponent<LevelData>(language);
	player.AddComponent<InputController>();
	player.AddComponent<ResourceComponent>(&textures[0], T_MAX, &sounds[0], SE_MAX, &currBGM[0], 1);
	player.AddComponent<TimeComponent>(&tick, &deltaTime, &gamePaused);
	player.AddComponent<TransformComponent>(116, 0);
	player.AddComponent<SpriteComponent>(&textures[T_PROTAG], &camera);
	player.AddComponent<EntityColliderComponent>(objPlayer);
	player.AddComponent<TileColliderComponent>(objPlayer, &levelData);
	player.AddComponent<PlayerComponent>(&levelData);
	player.AddComponent<PlayerDebugComponent>();

	//wall.AddComponent<TransformComponent>(370.0f, 377.0f, 8, 8, 1);
	//wall.AddComponent<SpriteComponent>(textures[T_PROTAG]);
	//wall.AddComponent<ColliderComponent>("wall");

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

/*
	if (Collision::AABB(player.GetComponent<ColliderComponent>().collider,
		wall.GetComponent<ColliderComponent>().collider)) {
		//player.GetComponent<TransformComponent>().scale = 1;
		player.GetComponent<TransformComponent>().velocity * -1;
		// Wall hit
	}
	*/

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
		// The game is ending; Free all game resources

		//Free sounds
		for (int i = 0; i < SE_MAX; i++) {
			PHL_FreeSound(sounds[i]);
		}
		
		//Free Music
		PHL_FreeMusic(currBGM[0]);
	}
	tick++;
}

void Game::Draw() {
	PHL_StartDrawing();
	PHL_DrawBackground(background, foreground);



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

	// Find the current room, given the room coordinates
	Room* currRoom = &levelData.GetComponent<LevelData>().world.area[levelData.GetComponent<LevelData>().currArea].room[currRoomX % 4][currRoomY % 5];

	// Switch room data based on the cardinal directions we've pressed (debug)
	int moveRoom = DIR_NONE;

	input = &player.GetComponent<InputController>().input;
	if (input->btnL.held) {
		if (input->btnFaceRight.pressed) {
			levelData.GetComponent<LevelData>().LoadMap(1, textures[T_ATEX]);
			PHL_FreeSurface(textures[T_ATEX]);
			texID = levelData.GetComponent<LevelData>().world.texID[levelData.GetComponent<LevelData>().currArea];
			textures[T_ATEX] = PHL_LoadTexture(texID);

			switch(levelData.GetComponent<LevelData>().currArea) {
				case MAP00:
					PHL_FreeMusic(currBGM[0]);
					currBGM[0] = PHL_LoadMusic("m01", loopBGM);
					PHL_PlayMusic(currBGM[0]);
					break;
				case MAP01:
					PHL_FreeMusic(currBGM[0]);
					currBGM[0] = PHL_LoadMusic("m00", loopBGM);
					PHL_PlayMusic(currBGM[0]);
					break;
			}
		} else if (input->btnFaceDown.pressed) {
			levelData.GetComponent<LevelData>().LoadMap(-1, textures[T_ATEX]);
			PHL_FreeSurface(textures[T_ATEX]);
			texID = levelData.GetComponent<LevelData>().world.texID[levelData.GetComponent<LevelData>().currArea];
			textures[T_ATEX] = PHL_LoadTexture(texID);
			switch(levelData.GetComponent<LevelData>().currArea) {
				case MAP00:
					PHL_FreeMusic(currBGM[0]);
					currBGM[0] = PHL_LoadMusic("m01", loopBGM);
					PHL_PlayMusic(currBGM[0]);
					break;
				case MAP01:
					PHL_FreeMusic(currBGM[0]);
					currBGM[0] = PHL_LoadMusic("m00", loopBGM);
					PHL_PlayMusic(currBGM[0]);
					break;
			}
		}

		if (input->btnRight.pressed)
			moveRoom = DIR_RIGHT;
		else if (input->btnLeft.pressed)
			moveRoom = DIR_LEFT;
		else if (input->btnUp.pressed)
			moveRoom = DIR_UP;
		else if (input->btnDown.pressed)
			moveRoom = DIR_DOWN;
	}

	float* playerX = &player.GetComponent<TransformComponent>().position.x;
	float* playerY = &player.GetComponent<TransformComponent>().position.y;
	float* playerBBoxLeft = &player.GetComponent<PlayerComponent>().bboxLeft;
	float* playerBBoxRight = &player.GetComponent<PlayerComponent>().bboxRight;
	float* playerBBoxTop = &player.GetComponent<PlayerComponent>().bboxTop;
	float* playerBBoxBottom = &player.GetComponent<PlayerComponent>().bboxBottom;
	int* pFacingX = &player.GetComponent<PlayerComponent>().facingX;
	float* pVsp = &player.GetComponent<PlayerComponent>().vsp;
	printf("%f\033[0;0H", *pVsp);
	if (*playerBBoxRight >= 254 && *pFacingX == DIR_RIGHT) {
		moveRoom = DIR_RIGHT;
	} else if (*playerBBoxLeft <= 2 && *pFacingX == DIR_LEFT) {
		moveRoom = DIR_LEFT;
	} else if (*playerBBoxBottom >= 190 && *pVsp > 0) {
		moveRoom = DIR_DOWN;
	} else if (*playerBBoxTop <= 2 && *pVsp < 0) {
		moveRoom = DIR_UP;
	}

	switch(moveRoom) {
		default:
			break;
		case DIR_LEFT: // Left
			if (currRoom->mapLeft.x >= 0 && currRoom->mapLeft.y >= 0) {	
				if (currRoom->mapLeft.x == -1 || currRoom->mapLeft.y == -1)
					break;
				levelData.GetComponent<LevelData>().currArea = currRoom->mapLeft.area;
				levelData.GetComponent<LevelData>().currRoomX = currRoom->mapLeft.x;
				levelData.GetComponent<LevelData>().currRoomY = currRoom->mapLeft.y;
				currRoomX = levelData.GetComponent<LevelData>().currRoomX;
				currRoomY = levelData.GetComponent<LevelData>().currRoomY;
				UpdateMapGraphics();
				UpdateMapBGM();
				*playerX = 253;
			}
			break;
		case DIR_RIGHT: // Right
			if (currRoom->mapRight.x >= 0 && currRoom->mapRight.y >= 0) {
				if (currRoom->mapRight.x == -1 || currRoom->mapRight.y == -1)
					break;
				levelData.GetComponent<LevelData>().currArea = currRoom->mapRight.area;
				levelData.GetComponent<LevelData>().currRoomX = currRoom->mapRight.x;
				levelData.GetComponent<LevelData>().currRoomY = currRoom->mapRight.y;
				currRoomX = levelData.GetComponent<LevelData>().currRoomX;
				currRoomY = levelData.GetComponent<LevelData>().currRoomY;
				UpdateMapGraphics();
				UpdateMapBGM();
				*playerX = 3;
			}
			break;
		case DIR_UP: // Up
			if (currRoom->mapUp.x >= 0 && currRoom->mapUp.y >= 0) {
				if (currRoom->mapUp.x == -1 || currRoom->mapUp.y == -1)
					break;
				levelData.GetComponent<LevelData>().currArea = currRoom->mapUp.area;
				levelData.GetComponent<LevelData>().currRoomX = currRoom->mapUp.x;
				levelData.GetComponent<LevelData>().currRoomY = currRoom->mapUp.y;
				currRoomX = levelData.GetComponent<LevelData>().currRoomX;
				currRoomY = levelData.GetComponent<LevelData>().currRoomY;
				UpdateMapGraphics();
				UpdateMapBGM();
				*playerY = 187;
			}
			break;
		case DIR_DOWN: // Down
			if (currRoom->mapDown.x >= 0 && currRoom->mapDown.y >= 0) {
				if (currRoom->mapDown.x == -1 || currRoom->mapDown.y == -1)
					break;
				levelData.GetComponent<LevelData>().currArea = currRoom->mapDown.area;
				levelData.GetComponent<LevelData>().currRoomX = currRoom->mapDown.x;
				levelData.GetComponent<LevelData>().currRoomY = currRoom->mapDown.y;
				currRoomX = levelData.GetComponent<LevelData>().currRoomX;
				currRoomY = levelData.GetComponent<LevelData>().currRoomY;
				UpdateMapGraphics();
				UpdateMapBGM();
				*playerY = 3;
			}
			break;
	}

	Area* _thisArea = &levelData.GetComponent<LevelData>().world.area[levelData.GetComponent<LevelData>().currArea];
	Room* _thisRoom = &_thisArea->room[currRoomX % 4][(currRoomY + 5) % 5];
/*
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

	printf("\nRoom: (%d, %d)\nRoom Group ID: %d                     \ntexID: %d                     \nUp %d,%d,%d   \nRight %d,%d,%d   \nDown %d,%d,%d   \nLeft: %d,%d,%d   \nUDLR - Move around the map\nA - Go forward a map\nB - Go back a map\033[0;0H", currRoomX % 4, currRoomY % 5, roomID, texID - 4, u1,u2,u3, r1,r2,r3, d1,d2,d3, l1,l2,l3);
*/

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
	
	float _camX = camera.GetComponent<CameraComponent>().x;
	float _camY = camera.GetComponent<CameraComponent>().y;
	int drawXOffset = floor(0 + _camX);
	int drawYOffset = floor(0 + _camY);

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
					PHL_DrawSurfacePart((_x * 8) + drawXOffset, (_y * 8) + drawYOffset, (_thisTileIndex % 40) * 8, (_thisTileIndex/40) * 8, 8, 8, textures[T_ATEX]); // Draw the first frame normally
				} else {
					PHL_DrawSurfacePart((_x * 8) + drawXOffset, (_y * 8) + drawYOffset, (_aniTiles->at(_thisAniIndex - 1) % 40) * 8, (_aniTiles->at(_thisAniIndex - 1)/40) * 8, 8, 8, textures[T_ATEX]); // For all the other frames, load the index dynamically
				}
			} else {
				PHL_DrawSurfacePart((_x * 8) + drawXOffset, (_y * 8) + drawYOffset, (_thisTileIndex % 40) * 8, (_thisTileIndex/40) * 8, 8, 8, textures[T_ATEX]);
			}
		}
	}
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