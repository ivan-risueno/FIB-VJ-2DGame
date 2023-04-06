#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <SOIL.h>

#define SCREEN_X 16*4
#define SCREEN_Y 16*0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 2

int readyX = 224;
int readyY = 224;

#define NLEVELS 3

int scorepoints;
int playerHealth;

Scene::Scene()
{
	menu = NULL;
	actualLevel = NULL;
	readyQuad = NULL;
	readyText = NULL;
	gameover = NULL;
}

Scene::~Scene()
{
	if (menu != NULL)
		delete menu;
	if (actualLevel != NULL)
		delete actualLevel;
	if (readyQuad != NULL)
		delete readyQuad;
	if (readyText != NULL)
		delete readyText;
	if (gameover != NULL)
		delete gameover;
}


void Scene::init()
{
	initShaders();
	menu = new MainMenu(texProgram);
	actualLevel = new Level(texProgram);
	menu->initSplash();

	music = new Sound();
	music->play("sounds/menu.wav", true, false);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1)/1.0f, float(SCREEN_HEIGHT - 1)/1.0f, 0.f);
	currentTime = 0.0f;
	pauseSecsRemaining = 0.f;
	playing = false;
	gameoverScreen = false;
	gameFinished = false;

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(192.f, 32.f) };
	glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	readyQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	readyText = new Texture();
	readyText->loadFromFile("images/ready.png", TEXTURE_PIXEL_FORMAT_RGBA);

	gameoverText.loadFromFile("images/spritesheets/GameoverSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameover = Sprite::createSprite(glm::ivec2(264, 32), glm::vec2(1, 0.25), &gameoverText, &texProgram);
	gameover->setNumberAnimations(1);

	gameover->setAnimationSpeed(0, 1);
	for (int i = 0; i < 4; ++i) {
		gameover->addKeyframe(0, glm::vec2(0.f, i * 0.25f));
	}
	gameover->changeAnimation(0);
	gameover->setPosition(glm::vec2(192.f, 320.f));
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!playing) {
		menu->update(deltaTime);
		if (menu->startPlaying()) {
			selectLevel(1);
			playing = true;
			setPause(3);
		}
	}
	else if (actualLevel->gameover()) {
		if (!gameover->finalKeyframe()) gameover->update(deltaTime);
		if (Game::instance().getKey(13)) {
			music->stop();
			init();
		}
	}
	else {
		if (Game::instance().getKey(49)) {	// Tecla 1
			selectLevel(1);
		}
		else if (Game::instance().getKey(50)) {	// Tecla 2
			selectLevel(2);
		}
		if (Game::instance().getKey(51)) {	// Tecla 3
			selectLevel(3);
		}

		decrementPause(deltaTime);
		if (!paused()) actualLevel->update(deltaTime);

		if (actualLevel->levelFinished() && currentLevel != NLEVELS) {
			selectLevel(currentLevel + 1);
		}
		else if (actualLevel->levelFinished() && currentLevel == NLEVELS) {
			gameFinished = true;
			music->play("sounds/credits.wav", true, false);
		}

	}

	if (playing && !gameoverScreen && actualLevel->gameover() && !gameFinished) {
		gameoverScreen = true;
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
		glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		readyQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
		readyText = new Texture();
		readyText->loadFromFile("images/mainMenu/gameoverScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
		music->play("sounds/deadScreen.wav", true, false);
	}
}

void Scene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	if (!playing) menu->render();
	else if (gameFinished) {
		menu->showCredits();
		menu->render();
	}
	else if (gameoverScreen) {
		texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
		readyQuad->render(*readyText);	// Not the ready prompt, just the gameover screen
		gameover->render();
	}
	else { 
		actualLevel->render();
		if (paused()) {
			glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(readyX, readyY, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			readyQuad->render(*readyText);
		}
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::selectLevel(int lvl) {
	actualLevel == NULL ? scorepoints = 0 : scorepoints = actualLevel->getScorepoints();
	actualLevel == NULL ? playerHealth = 3 : playerHealth = actualLevel->getPlayerHealth();
	delete actualLevel;
	actualLevel = new Level(texProgram);
	switch (lvl) {
	case 1:
		currentLevel = 1;
		actualLevel->init01(scorepoints, playerHealth);
		music->play("sounds/level01.wav", true, false);
		break;
	case 2:
		currentLevel = 2;
		actualLevel->init02(scorepoints, playerHealth);
		music->play("sounds/level02.wav", true, false);
		break;
	case 3:
		currentLevel = 3;
		actualLevel->init03(scorepoints, playerHealth);
		music->play("sounds/level03.wav", true, false);
		break;
	default: // game finished
		playing = false;
		delete actualLevel;
		break;
	}
	setPause(5);
}

void Scene::setPause(int seconds) {
	pauseSecsRemaining = seconds;
}

void Scene::decrementPause(int deltaTime) {
	pauseSecsRemaining -= float(deltaTime) / 1000;
}

bool Scene::paused() {
	return pauseSecsRemaining > 0;
}

