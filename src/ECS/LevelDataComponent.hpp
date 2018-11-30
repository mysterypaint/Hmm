/// Forewarning: Any reference to the XML uses the variable names used in the original game's assets.

#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include "Components.hpp"
#include "../Resources.hpp"
#include "../LinkedList.hpp"
using namespace std;

class LevelData : public Component {
private:
	int language;
	int currXMLWorld = 0;		// Which world the XML is focusing on while parsing the txt data
	int currXMLField = MAP00;	// Aka this source's "Area"
	int currXMLMapX = 0;	// Aka this source's "RoomX"
	int currXMLMapY = 0;	// Aka this source's "RoomY"
	InputListener input;
public:
	World world;
	Area currMap;
	int currArea = MAP01;		// The actual game's currently loaded map (Aka "FIELD" in the XML data)
	int currRoomX = 3;			// The actual game's current room X coordinate
	int currRoomY = 1;			// The actual game's current room Y coordinate
	std::map<std::string, XMLControlCommands> s_controlCommandArray;
	List animatedTilesList;

	LevelData(int _language) {
		language = _language;
	}

	~LevelData() { // As LevelData is being destroyed...
		// ... Delete every single object from every single room of every single area by deallocating them all
		/*

		//for (int i = MAP00; i < MAP_MAX; i++) {
		//	for (int _y = 0; _y < 5; _y++) {
		//		for (int _x = 0; _x < 4; _x++) {
		//			vctr<ObjectSpawnData>().swap(world.area[i].room[_x][_y].roomObjects);
		//		}
		//	}
		//	vctr<ObjectSpawnData>().swap(world.area[i].areaObjects);
		//}

		*/
	}

	void Create() override {
		InitLevelData();
		ParseXMLData(language);	// Based on the global game's language (EN, JP) we'll grab the dialogue/map data respective to that language
		DefineTileGFXArrays();
		world.areaPtr = &world.area[1];
		printf("LevelData: %x\n", &world);
		printf("LevelData AreaPTR: %x\n", world.areaPtr);
	}

	void Step() override {
		
	}

	void Draw() override {
		
	}

	World* GetWorld() {
		return &world;
	}

	//enum XMLControlCommands {CC_FIELD, CC_WORLD, CC_CHIPLINE, CC_HIT, CC_ANIME, CC_OBJECT, CC_START, CC_LEFT, CC_RIGHT, CC_UP, CC_DOWN, CC_MAP, CC_MAX};
	void ParseXMLData(int _language) {
		s_controlCommandArray["TALK"] = CC_TALK;
		s_controlCommandArray["WORLD"] = CC_WORLD;
		s_controlCommandArray["FIELD"] = CC_FIELD;
		s_controlCommandArray["MAP"] = CC_MAP;
		s_controlCommandArray["OBJECT"] = CC_OBJECT;
		s_controlCommandArray["LEFT"] = CC_LEFT;
		s_controlCommandArray["UP"] = CC_UP;
		s_controlCommandArray["RIGHT"] = CC_RIGHT;
		s_controlCommandArray["DOWN"] = CC_DOWN;
		s_controlCommandArray["HIT"] = CC_HIT;
		s_controlCommandArray["ANIME"] = CC_ANIME;

		// Before loading the actual content, initialize the texID[] array.
		for (int i = MAP00; i < MAP_MAX; i++)
			world.texID[i] = sprMapG00;

		string fName;

		switch(_language) {
			default:
			case ENG:
				fName = "romfs:/data/script_ENG.txt"; // English level/dialogue/animation/object spawn data
				break;
			case JPN:
				fName = "romfs:/data/script.txt"; // Japanese level/dialogue/animation/object spawn data
		}

		ifstream _f_in(fName);
		string line;
		string fContents;

		/*
		roomIDGroup.AddNode(3);
		roomIDGroup.AddNode(5);
		roomIDGroup.AddNode(6);
		roomIDGroup.AddNode(8);
		roomIDGroup.PrintList();
		roomIDGroup.DeleteNode(3);
		roomIDGroup.PrintList();
		*/


		while(getline(_f_in, line)) {
			line.erase(remove(line.begin(), line.end(), '\t'), line.end()); // Remove all tabs from every single line, just in case	
			
			int lineLength = line.size() - 1;

			for(int currLinePos = 0; currLinePos < lineLength; currLinePos++) {
				if (line.at(currLinePos) == '<') {
					currLinePos = GetXMLControlCode(line, currLinePos);
				}
			}
			fContents += line + "\n";
		}
		//printf(fContents.c_str());
	}

	int GetXMLControlCode(string _lineIn, int _currLinePos) {
		_currLinePos++;
		string controlCommand = "";
		bool commandEnd = false;
		bool disableCommand = false;
		std::vector<int> commandArgs;

		if (_lineIn.at(_currLinePos) == '/') { // If we hit a / immediately, we know the control code's trying to disable something.
			disableCommand = true;
			_currLinePos++;
		}

		int lineLength = _lineIn.length() - 1;
		while (_currLinePos < lineLength) {
			char _this_char = _lineIn.at(_currLinePos);
			if (_this_char == '>')
				break; // The end of a control code, the ]
			if (_this_char == ' ') { // The end of a control command, the :
				commandEnd = true;
				_currLinePos++;
			}
			if (!commandEnd)
				controlCommand += _this_char;
			else {
				// Grabbing all control code arguments here
				string _valueFound = "";
				while (_lineIn.at(_currLinePos) != ',' && _lineIn.at(_currLinePos) != '>') { // While we haven't hit a comma or a > ...
					_valueFound += _lineIn.at(_currLinePos);
					_currLinePos++;
				}
				int _val = atoi(_valueFound.c_str());
				commandArgs.push_back(_val);
			}
			if (_lineIn.at(_currLinePos) == '>')
				break;
			_currLinePos++;
		}

	/*

		// All the "out" stuff below is for debugging purposes, using the console output.
		string _out = "";
		if (disableCommand)
			_out += "[/";
		else _out += "[";

		_out += controlCommand + "] Arguments: (";

		int argCount = static_cast<int>(commandArgs.size());

		if (argCount <= 0 ) _out += "none)";
		else {
			std::stringstream ss;
			for (int i = 0; i < argCount; i++) {
				//_out += (string) commandArgs.at(i);
				const char base_string[] = "";
				char wow[100];
				sprintf(wow, "%s%d", base_string, commandArgs.at(i));

				_out += wow;
				if (i+1 >= argCount) _out += ")";
				else _out += ", ";
			}
		}

		printf("%s\n", _out.c_str());
	*/

		string _out = "";
		int argCount = static_cast<int>(commandArgs.size());
		int args[argCount];

		if (argCount > 0 ) {
			_out += ' ';
			std::stringstream ss;
			for (int i = 0; i < argCount; i++) {
				args[i] = commandArgs.at(i);
				//_out += (string) commandArgs.at(i);
				const char base_string[] = "";
				char wow[100];
				sprintf(wow, "%s%d", base_string, commandArgs.at(i));
				_out += wow;
				if (!(i+1 >= argCount))
					_out += ",";
			}
		}
		// Now, we need to make meaning out of all these commands.


		//CC_FIELD, CC_WORLD, CC_CHIPLINE, CC_HIT, CC_ANIME, CC_OBJECT, CC_START, CC_LEFT, CC_RIGHT, CC_UP, CC_DOWN, CC_MAP, CC_MAX
/*

			case CC_WORLD:
				currXMLWorld = commandArgs.at(0);
				break;
			case CC_FIELD:
				currXMLField = commandArgs.at(0);	// Aka this source's "Area"
				break;
				*/
		switch (s_controlCommandArray[controlCommand]) {
			default:
				break;
			case CC_FIELD:
				if (!disableCommand) {
					currXMLField = args[0];
					world.texID[currXMLField] = args[2] + sprMapG00;
					//printf("\nField: %d", currXMLField);
				}
				break;
			case CC_MAP: // This will set the roomGroupID of every single room
				if (!disableCommand) {
					currXMLMapX = args[0];	// Aka this source's "RoomX"
					currXMLMapY = args[1];	// Aka this source's "RoomY"
					DefineRoom(currXMLField, currXMLMapX, currXMLMapY, args[2]);
				}
				break;
			case CC_UP:
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapUp.world = args[0];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapUp.area = args[1];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapUp.x = args[2];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapUp.y = args[3];
				break;
			case CC_DOWN:
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapDown.world = args[0];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapDown.area = args[1];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapDown.x = args[2];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapDown.y = args[3];
				break;
			case CC_LEFT:
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapLeft.world = args[0];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapLeft.area = args[1];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapLeft.x = args[2];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapLeft.y = args[3];
				break;
			case CC_RIGHT:
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapRight.world = args[0];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapRight.area = args[1];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapRight.x = args[2];
				world.area[currXMLField].room[currXMLMapX][currXMLMapY].mapRight.y = args[3];
				break;
			case CC_ANIME:
				world.area[currXMLField].tileAnimationList[args[0]].waitFrames = args[1];
				for (int i = 2; i < argCount; i++) {
					world.area[currXMLField].tileAnimationList[args[0]].aniTiles.push_back(args[i]);
				}
				break;
		}

		return _currLinePos-1;
	}

	void DefineRoom(int _area, int _x, int _y, int _roomGroupID) {
		world.area[_area].room[_x][_y].roomGroupID = _roomGroupID;
	}

	void LoadMap(int _inc, PHL_Surface& _tex) {
		currArea += _inc;
		if (currArea < 0)
			currArea = MAP_MAX - 1;
		if (currArea > MAP_MAX - 1)
			currArea = 0;

		//PHL_FreeSurface(_tex);
		currMap = world.area[currArea];
		//_tex = PHL_LoadTexture(world.texID[currArea]);
	}
	void InitLevelData() {
		int fileSize = 0x6E00; // Assume every map file is this large (until we start supporting mods, I guess)

		for (int _area = MAP00; _area < MAP_MAX; _area++) { // For every single map...

			// ... Figure out the filename of the map we're trying to load
			std::string _f_in = "romfs:/data/";
			if (_area < BOSS00) {
				_f_in += "map";
				if (_area < MAP10)
					_f_in += "0";
				_f_in += std::to_string(_area);
			} else {
				_f_in += "boss0" + std::to_string(_area - BOSS00);
			}
			_f_in += ".dat";

			// Read the contents of that map
		    FILE * f;
		    if ((f = fopen(_f_in.c_str(), "rb"))) {
		        uint8_t* datFile = (uint8_t*) malloc(fileSize * sizeof(uint8_t));
		        fread(datFile, fileSize, 1, f);
		        fclose(f);

		        int roomWidth = 32;  // How many 8x8 tiles a room is wide
		        int roomHeight = 22; // How many 8x8 tiles a room is tall
		        int mapWidth = 4;	// How many rooms a map is wide (we don't need to know how tall it is since we have the filesize already)
                int roomX = 0;		// The Room X coordinate we're writing to
                int roomY = 0;		// The Room Y coordinate we're writing to
                int lineContents[roomWidth];

                for (int _offset = 0x0; _offset < fileSize; _offset += 0x2)								// Throughout the entire file,
                {
                    uint16_t _currTile = (uint16_t)((datFile[_offset + 1] << 8) | datFile[_offset]); // Grab every 2 bytes, in the order the file has them stored. Every 2 bytes represents 1 tile

                	int _i = _offset / 2; // To make this more readable/less confusing, let _i = "the current tile we're dealing with in the entire file"

                    lineContents[_i % roomWidth] = _currTile; // Every <roomWidth> tiles = 1 row of a room. Put the correct tileID in the correct relative row (we don't know what Y is yet)


                    if (_i % roomWidth == 0 && _i > 0) { // Besides the first tile, for every <roomWidth> tiles, increment roomX (make sure it never goes beyond the bounds of the map)
                    	roomX++;
                    }
					
					if (roomX > mapWidth - 1) {
						roomX = 0;

						if (_i % (roomWidth * roomHeight * mapWidth) == 0)
							roomY++;
					}

                    int _rtx = _i % roomWidth; // Relative Room Tile X
                    int _rty = _i / (roomWidth * mapWidth); // Relative Room Tile Y

                    world.area[_area].room[roomX][roomY].tileData[((_rty * roomWidth) + _rtx) % (roomWidth * roomHeight)].tileID = lineContents[_i % roomWidth];
                }

		        free(datFile);
			}
		}
	}

	void DefineTileGFXArrays() {
		/*
		world.texID[MAP00] = sprMapG00;
		world.texID[MAP01] = sprMapG01;
		world.texID[MAP02] = sprMapG02;
		world.texID[MAP03] = sprMapG03;
		world.texID[MAP04] = sprMapG04;
		world.texID[MAP05] = sprMapG05;
		world.texID[MAP06] = sprMapG06;
		world.texID[MAP07] = sprMapG07;
		world.texID[MAP08] = sprMapG08;
		world.texID[MAP09] = sprMapG09;
		world.texID[MAP10] = sprMapG10;
		world.texID[MAP11] = sprMapG11;
		world.texID[MAP12] = sprMapG12;
		world.texID[MAP13] = sprMapG13;
		world.texID[MAP14] = sprMapG14;
		world.texID[MAP15] = sprMapG15;
		world.texID[MAP16] = sprMapG16;
		world.texID[MAP17] = sprMapG17;
		world.texID[MAP18] = sprMapG18;
		world.texID[MAP19] = sprMapG19;

		world.texID[MAP20] = sprMapG18;
		world.texID[MAP21] = sprMapG18;
		world.texID[MAP22] = sprMapG18;
		world.texID[MAP23] = sprMapG18;
		world.texID[MAP24] = sprMapG18;
		world.texID[MAP25] = sprMapG18;
		world.texID[MAP26] = sprMapG20;
		world.texID[MAP27] = sprMapG20;
		world.texID[MAP28] = sprMapG21;
		world.texID[MAP29] = sprMapG05;
		world.texID[MAP30] = sprMapG18;
		world.texID[MAP31] = sprMapG31;
		world.texID[MAP32] = sprMapG32;
		world.texID[MAP33] = sprMapG20;
		world.texID[MAP34] = sprMapG22;*/


		world.texID[BOSS00] = sprMapG00;
		world.texID[BOSS01] = sprMapG02;
		world.texID[BOSS02] = sprMapG03;
		world.texID[BOSS03] = sprMapG04;
		world.texID[BOSS04] = sprMapG05;
		world.texID[BOSS05] = sprMapG06;
		world.texID[BOSS06] = sprMapG10;
		world.texID[BOSS07] = sprMapG17;
		world.texID[BOSS08] = sprMapG19;
	}
};