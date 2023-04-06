#ifndef _SCOREBOARD_INCLUDE
#define _SCOREBOARD_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "TexturedQuad.h"
#include "Sprite.h"


class Scoreboard
{

private:

public:

	Scoreboard(int nLifes, int score, int nStage);
	~Scoreboard();

	void initScoreboard(ShaderProgram texProgram);
	void update(int deltaTime, double currentTime, int score, int lifesLeft);
	void render();
	int getTimeLeft();
	int getScore();
	bool gameover();

private:
	int currentStage, timeLeft, points, playerLifes;
	ShaderProgram texProgram;
	TexturedQuad* baseQuad;
	Texture* baseTexture;

	Texture* numbers[10];
	TexturedQuad* numbersQuads[10];



};


#endif // _SCOREBOARD_INCLUDE