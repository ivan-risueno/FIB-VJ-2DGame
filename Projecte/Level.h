#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "Enemy.h"
#include "Bola.h"
#include "Skeleton.h"
#include "Vampire.h"
#include "Item.h"
#include "Door.h"
#include "Scoreboard.h"
#include "Sound.h"


class Level
{

public:
	Level(ShaderProgram texProgram);
	~Level();

	void init01(int scorepoints, int playerHealth);
	void init02(int scorepoints, int playerHealth);
	void init03(int scorepoints, int playerHealth);
	void update(int deltaTime);
	void render();
	bool checkDamage();
	int checkItems();
	glm::ivec2 getCordsPlayer();
	glm::ivec2 getCordsBola(int i);
	glm::ivec2 getCordsSkeleton(int i);
	glm::ivec2 getCordsVampire(int i);
	glm::ivec2 getCordsItem(int i);
	glm::ivec2 getCordsDoor();
	bool checkCollision(glm::ivec2 cords1, glm::ivec2 size1, glm::ivec2 cords2, glm::ivec2 size2);
	bool inside(glm::ivec2 cords, glm::ivec2 cords2, glm::ivec2 size2);
	bool levelFinished();
	int getScorepoints();
	int getPlayerHealth();
	bool gameover();
	

private:
	TileMap* map;
	Player* player;
	vector<Bola*> bolas;
	vector<Skeleton*> skeletons;
	vector<Vampire*> vamps;
	vector<Item*> items;
	Door* door;
	glm::ivec2 cordsSpikes;
	glm::ivec2 sizeSpikes;
	bool allTiles = false;
	bool renderPlayer = true;
	int score;
	bool stopwatch = false;
	int stopwatchTime = 200;
	bool damaged = false;
	bool immortal = false;
	int blinking = 0;
	int blinkTime = 120;
	int time = 0;
	bool enteringDoor = false;
	bool calcPoints = false;
	int calcPointsDelay = 120;
	bool finished;
	bool isGameover;
	int mortalTime = 0;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	TexturedQuad* backgroundQuad;
	Texture* backgroundTexture;
	Scoreboard* s;
	Sound* levelSound;
};


#endif // _LEVEL_INCLUDE

