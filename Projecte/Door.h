#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Door
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int mt);
	bool update(int deltaTime, int changedTiles);
	void render();
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosDoor();
	void open();
	bool checkClosed();

protected:

	bool closed;
	bool opening;
	int maxTiles;
	int currentTiles;
	glm::ivec2 tileMapDispl, posDoor;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};


#endif // _DOOR_INCLUDE