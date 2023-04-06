#ifndef _BOLA_INCLUDE
#define _BOLA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Bola : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right);
	int update(int deltaTime);
	void setPosition(const glm::vec2& pos, const glm::vec2& posM);

private:
	bool startingMove, colliding, melting, spawning;
	glm::ivec2 posBola, posMelt;
	
};


#endif // _BOLA_INCLUDE
