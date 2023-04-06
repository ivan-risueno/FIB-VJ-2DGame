#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Skeleton : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right);
	void update(int deltaTime);

private:

};


#endif // _SKELETON_INCLUDE