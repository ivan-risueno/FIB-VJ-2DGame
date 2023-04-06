#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"



enum DoorAnims
{
	CLOSE, OPEN, OPENING
};



void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int mt)
{
	closed = true;
	opening = false;
	maxTiles = mt;

	spritesheet.loadFromFile("images/spritesheets/DoorSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(CLOSE, 8);
	sprite->addKeyframe(CLOSE, glm::vec2(0.875f, 0.f));

	sprite->setAnimationSpeed(OPENING, 12);
	for (float i = 0.875; i >= 0.0; i -= 0.0625) {
		sprite->addKeyframe(OPENING, glm::vec2(i, 0.f));
	}

	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));

}

bool Door::update(int deltaTime, int changedTiles)
{
	bool allTiles = false;
	currentTiles += changedTiles;
	if (currentTiles > maxTiles) {
		allTiles = true;
	}
	if (currentTiles == maxTiles) {
		allTiles = true;
		currentTiles += 1;
	}

	sprite->update(deltaTime);

	if (closed) {
		if (sprite->animation() != CLOSE) {
			sprite->changeAnimation(CLOSE);
		}
	}
	else if (opening){
		if (sprite->animation() != OPENING) {
			sprite->changeAnimation(OPENING);
		}
		if (sprite->finalKeyframe()) {
			sprite->changeAnimation(OPEN);
			opening = false;
		}
	}
	return allTiles;
}

void Door::render()
{
	sprite->render();
}

void Door::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Door::setPosition(const glm::vec2& pos)
{
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

void Door::open() {
	closed = false;
	opening = true;
}

bool Door::checkClosed() {
	return closed;
}

glm::ivec2 Door::getPosDoor() {
	return posDoor;
}