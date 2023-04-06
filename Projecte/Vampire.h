#ifndef _VAMPIRE_INCLUDE
#define _VAMPIRE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Vampire : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right, bool fly);
	void update(int deltaTime);

private:
	bool flying;
	bool movingUp;
	int maxFloortime = 500;
	int floorTime;
	int maxDownHits = 4;
	int downHits;
};


#endif // _	VAMPIRE_INCLUDE
