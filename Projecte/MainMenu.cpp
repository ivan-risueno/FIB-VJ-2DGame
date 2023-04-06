#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <SOIL.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MainMenu.h"
#include <stdlib.h>

glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

MainMenu::MainMenu(ShaderProgram texProgram)
{
	play = false;
	splash = true;
	keyPressed = false;
	currentTime = 0;
	optionSelected = 0;
	this->texProgram = texProgram;
	screenQuad = NULL;
	screenTexture = NULL;
	selectorTexture = NULL;
	selector = NULL;
	menuSound = new Sound();
	extraQuad = NULL;
	extraTexture = NULL;
}

MainMenu::~MainMenu()
{
	if (screenQuad != NULL)
		delete screenQuad;
	if (screenTexture != NULL)
		delete screenTexture;
	if (selectorTexture != NULL)
		delete selectorTexture;
	if (selector != NULL)
		delete selector;
	if (extraQuad != NULL)
		delete extraQuad;
	if (extraTexture != NULL)
		delete extraTexture;
}


void MainMenu::initSplash()
{
	screenQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	screenTexture = new Texture();
	screenTexture->loadFromFile("images/backgrounds/titleScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);

	selectorTexture = new Texture();
	selectorTexture->loadFromFile("images/mainMenu/pressEnter.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selector = Sprite::createSprite(glm::ivec2(348.f, 30.f), glm::vec2(1, 0.5f), selectorTexture, &texProgram);
	selector->setNumberAnimations(1);

	selector->setAnimationSpeed(0, 2);
	selector->addKeyframe(0, glm::vec2(0.f, 0.f));
	selector->addKeyframe(0, glm::vec2(0.f, 0.5f));
	selector->changeAnimation(0);

	selector->setPosition(glm::vec2(146.f, 354.f));
}

void MainMenu::initMenu()
{
	showingCredits = false;
	showingInstructions = false;
	splash = false;
	screenQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	screenTexture = new Texture();
	screenTexture->loadFromFile("images/mainMenu/mainMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);

	selectorTexture = new Texture();
	selectorTexture->loadFromFile("images/mainMenu/vampSelector.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selector = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.3333f), selectorTexture, &texProgram);
	selector->setNumberAnimations(1);
	
	selector->setAnimationSpeed(0, 5);
	for (int i = 0; i < 3; ++i) {
		selector->addKeyframe(0, glm::vec2(0.f, i * 0.3333f));
	}
	selector->changeAnimation(0);

	extraTexture = new Texture();

	selectPlay();
}

void MainMenu::update(int deltaTime)
{
	currentTime += deltaTime;
	selector->update(deltaTime);
	if (splash && Game::instance().getKey(13)) {	// ENTER = 13(ASCII)
		menuSound->play("sounds/menuConfirm.wav", false);
		splash = false;
		keyPressed = true;
		initMenu();
		for (int i = 0; i < 100000000; ++i) {
			// Delay para evitar que con una pulsación de ENTER se entre al juego
		}
	}
	else if (!splash) {
		if (showingInstructions || showingCredits) {
			if (!keyPressed && Game::instance().getKey(13)) {
				keyPressed = true;
				showingInstructions = false;
				showingCredits = false;
			}
		} else {
			if (!keyPressed && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				menuSound->play("sounds/menuSlide.wav", false);
				if (optionSelected == 0) {
					optionSelected = 3;
					selectExit();
				}
				else if (optionSelected == 1) {
					optionSelected = 0;
					selectPlay();
				}
				else if (optionSelected == 2) {
					optionSelected = 1;
					selectInstructions();
				}
				else if (optionSelected == 3) {
					optionSelected = 2;
					selectCredits();
				}
				keyPressed = true;
			}
			else if (!keyPressed && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				menuSound->play("sounds/menuSlide.wav", false);
				if (optionSelected == 0) {
					optionSelected = 1;
					selectInstructions();
				}
				else if (optionSelected == 1) {
					optionSelected = 2;
					selectCredits();
				}
				else if (optionSelected == 2) {
					optionSelected = 3;
					selectExit();
				}
				else if (optionSelected == 3) {
					optionSelected = 0;
					selectPlay();
				}
				keyPressed = true;
			}
			else if (!keyPressed && Game::instance().getKey(13)) {
				menuSound->play("sounds/menuConfirm.wav", false);
				if (optionSelected == 0) {
					play = true;
				}
				else if (optionSelected == 1) {
					showingInstructions = true;
					extraQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
					extraTexture->loadFromFile("images/instructions/instructionsScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
				}
				else if (optionSelected == 2) {
					showingCredits = true;
					extraQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
					extraTexture->loadFromFile("images/credits/creditsScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
				}
				else if (optionSelected == 3) {
					Game::instance().keyPressed(27);	// Escape code
				}
				keyPressed = true;
			}
		}
		
		if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getKey(13)) keyPressed = false;
	}
}

void MainMenu::selectPlay()
{
	selector->setPosition(glm::vec2(208.f, 256.f));
}

void MainMenu::selectInstructions()
{
	selector->setPosition(glm::vec2(80.f, 304.f));
}

void MainMenu::selectCredits()
{
	selector->setPosition(glm::vec2(160.f, 352.f));
}

void MainMenu::selectExit()
{
	selector->setPosition(glm::vec2(128.f, 400.f));
}

void MainMenu::render()
{
	if (showingInstructions || showingCredits ) {
		extraQuad->render(*extraTexture);
	} else {
		screenQuad->render(*screenTexture);
		selector->render();
	}
}

bool MainMenu::startPlaying() const
{
	return play;
}

bool MainMenu::Splash() const
{
	return splash;
}

void MainMenu::showCredits() {
	if (!showingCredits) {
		extraQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
		extraTexture->loadFromFile("images/credits/creditsScreen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	showingCredits = true;
}