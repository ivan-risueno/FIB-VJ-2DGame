#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MainMenu.h"
#include "Level.h"
#include "Sound.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void selectLevel(int lvl);
	void setPause(int seconds);
	void decrementPause(int deltaTime);
	bool paused();

private:
	void initShaders();

private:
	bool playing, gameoverScreen, gameFinished;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	MainMenu *menu;
	Level* actualLevel;
	Sound* music;
	float pauseSecsRemaining;

	TexturedQuad *readyQuad;
	Texture *readyText;
	Sprite* gameover;
	Texture gameoverText;

	int currentLevel;
};


#endif // _SCENE_INCLUDE

