#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Sound.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int initialHealth);
	int update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosPlayer();
	int getHealth();
	void setHealth(int newHealth);
	void enterDoor(int deltaTime);
	bool enteredDoor();
	void die();
	bool alreadyDead();
	
private:
	int health;
	bool startingMove;
	bool bJumping, enteringDoor, alreadyEnteredDoor, dying, dead;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Sound* playerSound;

};


#endif // _PLAYER_INCLUDE


