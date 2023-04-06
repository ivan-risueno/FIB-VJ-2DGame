#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Item.h"
#include "Game.h"


enum ItemAnims
{
	HIDE, IDLE, APPEAR, DISAPPEAR
};



void Item::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type, glm::ivec2 showTime)
{
	show = showTime;
	hidden = true;
	taken = false;
	enabled = false;
	
	switch (type) {
	case 0:
		spritesheet.loadFromFile("images/spritesheets/KeySheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case 1:
		spritesheet.loadFromFile("images/spritesheets/GemSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case 2:
		spritesheet.loadFromFile("images/spritesheets/HeartSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case 3:
		spritesheet.loadFromFile("images/spritesheets/StopwatchSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	}

	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(HIDE, 8);
	sprite->addKeyframe(HIDE, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(IDLE, 4);
	for (float i = 0.0625; i <= 0.6875; i += 0.0625) {
		sprite->addKeyframe(IDLE, glm::vec2(i, 0.f));
	}

	sprite->setAnimationSpeed(APPEAR, 8);
	for (float i = 0.1875; i <= 0.3125; i += 0.0625) {
		sprite->addKeyframe(APPEAR, glm::vec2(i, 0.125f));
	}
	sprite->addKeyframe(APPEAR, glm::vec2(0.f, 0.125f));

	sprite->setAnimationSpeed(DISAPPEAR, 8);
	for (float i = 0.0; i <= 0.1875; i += 0.0625) {
		sprite->addKeyframe(DISAPPEAR, glm::vec2(i, 0.125f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));

}

void Item::update(int deltaTime, int type)
{
	sprite->update(deltaTime);

	if (hidden) {
		if (sprite->animation() == IDLE) {
			sprite->changeAnimation(DISAPPEAR);
		}
		if (sprite->finalKeyframe()) {
			sprite->changeAnimation(HIDE);
		}
	}
	else {
		if (sprite->animation() == HIDE) {
			sprite->changeAnimation(APPEAR);
		}
		else if (sprite->finalKeyframe()) {
			sprite->changeAnimation(IDLE);
		}
	}

	if (type != 0) {
		++time;
		if ((time >= show[0] && time <= show[1]) && !taken && !enabled) {
			hidden = false;
			enabled = true;
		}
		else if(!enabled) {
			hidden = true;
		}
	}
}

void Item::render()
{
	sprite->render();
}

void Item::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2& pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

glm::ivec2 Item::getPosItem() {
	return posItem;
}

bool Item::available() {
	return !hidden;
}

void Item::enable() {
	hidden = false;
	enabled = true;
}

void Item::disable() {
	hidden = true;
}

bool Item::checkEnabled() {
	return enabled;
}

void Item::take() {
	taken = true;
	hidden = true;
}