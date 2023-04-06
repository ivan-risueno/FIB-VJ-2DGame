#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Sound.h"




class Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosEnemy();

protected:

	bool startingMove;
	bool movingRight;
	glm::ivec2 tileMapDispl, posEnemy;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Sound* enemySound;

};


#endif // _ENEMY_INCLUDE
