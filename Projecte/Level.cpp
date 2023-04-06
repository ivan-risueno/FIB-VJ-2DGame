#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Level.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL.h>
#include "Game.h"


int SCREEN_X;
int SCREEN_Y;

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 2

float backgroundX;
float backgroundY;

Level::Level(ShaderProgram texProgram)
{
	map = NULL;
	player = NULL;
	this->texProgram = texProgram;
	currentTime = 0;
	finished = false;
	isGameover = false;
	backgroundQuad = NULL;
	backgroundTexture = NULL;
	s = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (backgroundQuad != NULL)
		delete backgroundQuad;
	if (backgroundTexture != NULL)
		delete backgroundTexture;
	if (s != NULL)
		delete s;

}


void Level::init01(int scorepoints, int playerHealth)
{
	levelSound = new Sound();
	SCREEN_X = 16 * 4;
	SCREEN_Y = 0;
	score = scorepoints;
	s = new Scoreboard(playerHealth, score, 1);
	s->initScoreboard(texProgram);
	score = 0;

	//---------------------------------------------------------------------------------------------------------------------
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(416.f, 400.f) };
	glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	backgroundQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	backgroundTexture = new Texture();
	backgroundTexture->loadFromFile("images/backgrounds/back1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundX = 112.f;
	backgroundY = 48.f;
	
	player = new Player();
	player->init(glm::ivec2(16 * 4, 0), texProgram, playerHealth);
	player->setPosition(glm::vec2(50,64));
	player->setTileMap(map);

	 
	skeletons = vector<Skeleton*>(2);

	skeletons[0] = new Skeleton();
	skeletons[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	skeletons[0]->setPosition(glm::vec2(125,256));
	skeletons[0]->setTileMap(map);

	skeletons[1] = new Skeleton();
	skeletons[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	skeletons[1]->setPosition(glm::vec2(305, 128));
	skeletons[1]->setTileMap(map);

	

	door = new Door();
	door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 66);
	door->setPosition(glm::vec2(370, 64));
	door->setTileMap(map);

	items = vector<Item*>(4);

	items[0] = new Item();
	items[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, glm::ivec2(0,0));
	items[0]->setPosition(glm::vec2(50, 64));
	items[0]->setTileMap(map);

	items[1] = new Item();
	items[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, glm::ivec2(300, 1000));
	items[1]->setPosition(glm::vec2(50, 320));
	items[1]->setTileMap(map);

	items[2] = new Item();
	items[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, glm::ivec2(700, 1400));
	items[2]->setPosition(glm::vec2(400, 320));
	items[2]->setTileMap(map);

	items[3] = new Item();
	items[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3, glm::ivec2(1000, 1900));
	items[3]->setPosition(glm::vec2(225, 120));
	items[3]->setTileMap(map);

	//---------------------------------------------------------------------------------------------------------------------


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 1.0f, float(SCREEN_HEIGHT - 1) / 1.0f, 0.f);
	currentTime = 0.0f;
}


void Level::init02(int scorepoints, int playerHealth)
{
	levelSound = new Sound();
	SCREEN_X = 0;
	SCREEN_Y = 0;
	score = scorepoints;
	s = new Scoreboard(playerHealth, score, 2);
	s->initScoreboard(texProgram);
	//---------------------------------------------------------------------------------------------------------------------
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(0, 0), texProgram);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	backgroundQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	backgroundTexture = new Texture();
	backgroundTexture->loadFromFile("images/backgrounds/back2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundX = 0;
	backgroundY = 0;
	
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram, playerHealth);
	player->setPosition(glm::vec2(38, 384));
	player->setTileMap(map);

	bolas = vector<Bola*>(2);

	bolas[0] = new Bola();
	bolas[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	bolas[0]->setPosition(glm::vec2(270, 96), glm::vec2(322, 384));
	bolas[0]->setTileMap(map);

	bolas[1] = new Bola();
	bolas[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	bolas[1]->setPosition(glm::vec2(307, 96), glm::vec2(255, 384));
	bolas[1]->setTileMap(map);
	//---------------------------------------------------------------------------------------------------------------------
	items = vector<Item*>(4);

	items[0] = new Item();
	items[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, glm::ivec2(0, 0));
	items[0]->setPosition(glm::vec2(512, 192));
	items[0]->setTileMap(map);

	items[1] = new Item();
	items[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, glm::ivec2(1000, 1900));
	items[1]->setPosition(glm::vec2(288, 256));
	items[1]->setTileMap(map);

	items[2] = new Item();
	items[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, glm::ivec2(300, 1000));
	items[2]->setPosition(glm::vec2(64, 192));
	items[2]->setTileMap(map);

	items[3] = new Item();
	items[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3, glm::ivec2(700, 1400));
	items[3]->setPosition(glm::vec2(288, 96));
	items[3]->setTileMap(map);

	door = new Door();
	door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 60);
	door->setPosition(glm::vec2(289, 384));
	door->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 1.0f, float(SCREEN_HEIGHT - 1) / 1.0f, 0.f);
	currentTime = 0.0f;
}


void Level::init03(int scorepoints, int playerHealth)
{
	levelSound = new Sound();
	SCREEN_X = 16 * 4;
	SCREEN_Y = 16 * 2;
	score = scorepoints;
	s = new Scoreboard(playerHealth, score, 3);
	s->initScoreboard(texProgram);
	//---------------------------------------------------------------------------------------------------------------------
	map = TileMap::createTileMap("levels/level03.txt", glm::vec2(64, 32), texProgram);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(512.f, 448.f) };
	glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	backgroundQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	backgroundTexture = new Texture();
	backgroundTexture->loadFromFile("images/backgrounds/back3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundX = 64;
	backgroundY = 32;
	
	player = new Player();
	player->init(glm::ivec2(64, 32), texProgram, playerHealth);
	player->setPosition(glm::vec2(128, 32));
	player->setTileMap(map);

	skeletons = vector<Skeleton*>(2);

	skeletons[0] = new Skeleton();
	skeletons[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	skeletons[0]->setPosition(glm::vec2(120, 352));
	skeletons[0]->setTileMap(map);

	skeletons[1] = new Skeleton();
	skeletons[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	skeletons[1]->setPosition(glm::vec2(330, 352));
	skeletons[1]->setTileMap(map);

	vamps = vector<Vampire*>(2);

	vamps[0] = new Vampire();
	vamps[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false, false);
	vamps[0]->setPosition(glm::vec2(135, 160));
	vamps[0]->setTileMap(map);

	vamps[1] = new Vampire();
	vamps[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true, false);
	vamps[1]->setPosition(glm::vec2(300, 160));
	vamps[1]->setTileMap(map);
	//---------------------------------------------------------------------------------------------------------------------
	
	items = vector<Item*>(4);

	items[0] = new Item();
	items[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0, glm::ivec2(0, 0));
	items[0]->setPosition(glm::vec2(128, 352));
	items[0]->setTileMap(map);

	items[1] = new Item();
	items[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, glm::ivec2(700, 1400));
	items[1]->setPosition(glm::vec2(290, 165));
	items[1]->setTileMap(map);

	items[2] = new Item();
	items[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, glm::ivec2(1100, 1900));
	items[2]->setPosition(glm::vec2(320, 352));
	items[2]->setTileMap(map);

	items[3] = new Item();
	items[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3, glm::ivec2(300, 1000));
	items[3]->setPosition(glm::vec2(150, 165));
	items[3]->setTileMap(map);

	
	door = new Door();
	door->init(glm::ivec2(0, 0), texProgram, 86);
	door->setPosition(glm::vec2(368, 64));
	door->setTileMap(map);

	cordsSpikes = glm::ivec2(176, 432);
	sizeSpikes = glm::ivec2(200, 16);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1) / 1.0f, float(SCREEN_HEIGHT - 1) / 1.0f, 0.f);
	currentTime = 0.0f;
}

void Level::update(int deltaTime)
{
	//if (player->enteredDoor()) finished = true;

	if (!stopwatch && !enteringDoor) {
		currentTime += deltaTime;
	}

	int changedTiles = 0;

	changedTiles += player->update(deltaTime);
	if (!enteringDoor) {
		if (!stopwatch) {
			for (int i = 0; i < skeletons.size(); ++i) skeletons[i]->update(deltaTime);
			for (int i = 0; i < bolas.size(); ++i) changedTiles -= bolas[i]->update(deltaTime);
			for (int i = 0; i < vamps.size(); ++i) vamps[i]->update(deltaTime);
		}
		for (int i = 0; i < items.size(); ++i) items[i]->update(deltaTime, i);
	}

	allTiles = door->update(deltaTime, changedTiles);
	score += 10*changedTiles;

	if (changedTiles != 0) {
		map->prepareArrays(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}

	if (allTiles || Game::instance().getKey(107)) {
		changedTiles = 0;
		if (!items[0]->checkEnabled()) {
			items[0]->enable();
		}
	}

	if (Game::instance().getKey(104)) {
		if (!items[2]->checkEnabled()) {
			items[2]->enable();
		}
	}

	int item = checkItems();

	switch (item) {
	case 0:
		levelSound->play("sounds/key.wav", false);
		if (door->checkClosed()) {
			door->open();
		}
		items[item]->take();
		break;
	case 1:
		levelSound->play("sounds/gem.wav", false);
		score += 100;
		items[item]->take();
		break;
	case 2:
		levelSound->play("sounds/heart.wav", false);
		player->setHealth(player->getHealth() + 1);
		items[item]->take();
		break;
	case 3:
		levelSound->play("sounds/stopwatch.wav", false);
		stopwatch = true;
		items[item]->take();
		break;
	}

	if (stopwatch && time < stopwatchTime) {
		++time;
		if (time == stopwatchTime) {
			stopwatch = false;
		}
	}

	if (Game::instance().getKey(103)) {
		if (immortal) {
			if (blinking >= 10) {
				immortal = false;
				mortalTime = 0;
			}
		}
		else {
			if (mortalTime >= 10) {
				blinking = 0;
				immortal = true;
			}
		}
	}

	if (!immortal) {
		++mortalTime;
		renderPlayer = true;
		if (checkDamage() && player->getHealth() != -1) {
			levelSound->play("sounds/damage.wav", false);
			player->setHealth(player->getHealth() - 1);
			if (player->getHealth() == -1) {
				levelSound->play("sounds/dead.wav", false);
			}
			immortal = true;
			damaged = true;
			blinking = 0;
		}
		if (!player->alreadyDead() && player->getHealth() == -1) {
			player->die();
		}
		if (player->alreadyDead()) {
			isGameover = true;
		}
	}
	else {
		++blinking;			
		if (blinking == blinkTime) {
			if (damaged) {
				immortal = false;
				damaged = false;
				renderPlayer = true;
				mortalTime = 0;
			}			
		}
		else {
			renderPlayer = (blinking % 14 < 7);
		}
	}

	if (!door->checkClosed()) {
		glm::vec2 cordsPlayer = getCordsPlayer();
		glm::vec2 cordsDoor = getCordsDoor();
		if (vamps.size() == 2) cordsDoor[0] -= 48;
		if (checkCollision(cordsPlayer, glm::ivec2(12, 24), cordsDoor, glm::ivec2(16, 32))) {
			enteringDoor = true;
			player->enterDoor(deltaTime);
		}
	}

	if (enteringDoor && !calcPoints) {
		if(calcPointsDelay == 119) levelSound->play("sounds/laugh.wav", false);
		--calcPointsDelay;
		if (calcPointsDelay == 0) calcPoints = true;
	}

	if (calcPoints) {
		int time = s->getTimeLeft();
		if (time == 0) {
			enteringDoor = false;
			calcPoints = false;
			calcPointsDelay = 120;
			finished = true;
			for (int i = 0; i < 999999999; ++i);
		}
		else {
			levelSound->play("sounds/gem.wav", false);
			currentTime += 1000;
			score += 10;
		}
	}

	if (int time = s->getTimeLeft() == 0) {
		isGameover = true;
		player->die();
	}
		
	
	s->update(deltaTime, currentTime / 1000, score, player->getHealth());
}

void Level::render()
{

	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(backgroundX, backgroundY, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	backgroundQuad->render(*backgroundTexture);

	texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	map->render();
	if (!isGameover) s->render();	// tal vez da error aqui que cuando te mueras en el tercer nivel haya una excepcion
	door->render();
	if(!enteringDoor){
		for (int i = 0; i < items.size(); ++i) items[i]->render();
		for (int i = 0; i < skeletons.size(); ++i) skeletons[i]->render();
		for (int i = 0; i < bolas.size(); ++i) bolas[i]->render();
		for (int i = 0; i < vamps.size(); ++i) vamps[i]->render();
	}
	if (renderPlayer) {
		player->render();
	}
	
}

bool Level::checkDamage() {
	glm::ivec2 cordsPlayer = getCordsPlayer();
	glm::ivec2 cordsEnemy;
	for (int i = 0; i < 4; ++i) {
		switch (i) {
		case 0:
			
			for (int j = 0; j < bolas.size(); ++j) {
				cordsEnemy = getCordsBola(j);
				if (checkCollision(cordsPlayer, glm::ivec2(12, 24), cordsEnemy, glm::ivec2(16, 16))) {
					return true;
				}
			}
			break;
		case 1:
			for (int j = 0; j < skeletons.size(); ++j) {
				cordsEnemy = getCordsSkeleton(j);
				if (checkCollision(cordsPlayer, glm::ivec2(12, 24), cordsEnemy, glm::ivec2(16, 32))) {
					return true;
				}
			}
			break;
		case 2:
			for (int j = 0; j < vamps.size(); ++j) {
				cordsEnemy = getCordsVampire(j);
				if (checkCollision(cordsPlayer, glm::ivec2(12, 24), cordsEnemy, glm::ivec2(16, 32))) {
					return true;
				}
			}
			break;
		case 3:
			if (checkCollision(cordsPlayer, glm::ivec2(12, 24), cordsSpikes, sizeSpikes)) {
				return true;
			}
		}
	}
	return false;
}

glm::ivec2 Level::getCordsPlayer() {
	glm::ivec2 cordsPlayer;
	cordsPlayer[0] = player->getPosPlayer()[0] + 23;
	cordsPlayer[1] = player->getPosPlayer()[1] + 25;
	return cordsPlayer;
}

glm::ivec2 Level::getCordsBola(int i ) {
	glm::ivec2 cordsBola;
	cordsBola[0] = bolas[i]->getPosEnemy()[0] + 24;
	cordsBola[1] = bolas[i]->getPosEnemy()[1] + 24;
	return cordsBola;
}

glm::ivec2 Level::getCordsSkeleton(int i) {
	glm::ivec2 cordsSkeleton;
	cordsSkeleton[0] = skeletons[i]->getPosEnemy()[0] + 24;
	cordsSkeleton[1] = skeletons[i]->getPosEnemy()[1] + 32;
	return cordsSkeleton;
}

glm::ivec2 Level::getCordsVampire(int i) {
	glm::ivec2 cordsVampire;
	cordsVampire[0] = vamps[i]->getPosEnemy()[0] + 32;
	cordsVampire[1] = vamps[i]->getPosEnemy()[1] + 32;
	return cordsVampire;
}

glm::ivec2 Level::getCordsItem(int i) {
	glm::ivec2 cordsItem;
	cordsItem[0] = items[i]->getPosItem()[0] + 32;
	cordsItem[1] = items[i]->getPosItem()[1] + 32;
	return cordsItem;
}

glm::ivec2 Level::getCordsDoor() {
	glm::ivec2 cordsDoor;
	cordsDoor[0] = door->getPosDoor()[0] + 32;
	cordsDoor[1] = door->getPosDoor()[1] + 32;
	return cordsDoor;
}

bool Level::checkCollision(glm::ivec2 cords1, glm::ivec2 size1, glm::ivec2 cords2, glm::ivec2 size2) {

	glm::ivec2 cords = cords1;

	for (int i = 0; i < 4; ++i) {
		switch (i) {
		case 0:
			if (inside(cords, cords2, size2)) {
				return true;
			}
			break;
		case 1:
			cords[0] += size1[0];
			if (inside(cords, cords2, size2)) {
				return true;
			}
			break;
		case 2:
			cords[1] += size1[1];
			if (inside(cords, cords2, size2)) {
				return true;
			}
			break;
		case 3:
			cords[0] += size1[0];
			cords[1] += size1[1];
			if (inside(cords, cords2, size2)) {
				return true;
			}
			break;
		}
	}
	return false;
}

bool Level::inside(glm::ivec2 cords, glm::ivec2 cords2, glm::ivec2 size2) {
	if (((cords[0] >= cords2[0]) && (cords[0] <= cords2[0] + size2[0])) &&
		((cords[1] >= cords2[1]) && (cords[1] <= cords2[1] + size2[1]))) {
		return true;
	}
	return false;
}

int Level::checkItems() {
	glm::ivec2 cordsPlayer = getCordsPlayer();
	glm::ivec2 cordsItem;
	for (int i = 0; i < items.size(); ++i) {
		cordsItem = getCordsItem(i);
		if (checkCollision(cordsPlayer, glm::ivec2(16, 24), cordsItem, glm::ivec2(24, 32))) {
			if (items[i]->available()) {
				return i;
			}
		}
	}
	return -1;
}

bool Level::levelFinished() {
	return finished;
}

int Level::getScorepoints() {
	return score;
}

int Level::getPlayerHealth() {
	return player == NULL ? 3 : player->getHealth();
}

bool Level::gameover() {
	return isGameover || (s->gameover() && finished);
}