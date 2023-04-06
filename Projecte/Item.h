#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type, glm::ivec2 showTime);
	void update(int deltaTime, int type);
	void render();
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosItem();
	bool available();
	void enable();
	void disable();
	bool checkEnabled();
	void take();

protected:

	glm::ivec2 tileMapDispl, posItem;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool hidden;
	bool taken;
	bool enabled;
	glm::ivec2 show;
	int time = 0;

};


#endif // _ITEM_INCLUDE
