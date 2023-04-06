#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4	// Saltar mas rapido o no
#define JUMP_HEIGHT 64
#define FALL_STEP 3			// Saltar mas rapido o no


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, FALL_RIGHT, FALL_LEFT, CAPE_LEFT, CAPE_RIGHT, ENTERING_DOOR, DYING
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int initialHealth)
{
	playerSound = new Sound();
	health = initialHealth;
	startingMove = true;
	bJumping = false;
	enteringDoor = false;
	alreadyEnteredDoor = false;
	dying = false;
	dead = false;
	spritesheet.loadFromFile("images/spritesheets/AlucardSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

	sprite->setAnimationSpeed(STAND_LEFT, 6);
	for (int i = 1; i <= 6; ++i) {
		sprite->addKeyframe(STAND_LEFT, glm::vec2(1 - i * 0.0625f, 0.f));
	}

	sprite->setAnimationSpeed(STAND_RIGHT, 6);
	for (int i = 0; i < 6; ++i) {
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(i * 0.0625f, 0.f));
	}

	sprite->setAnimationSpeed(CAPE_LEFT, 24);
	for (int i = 16; i >= 0; --i) {
		sprite->addKeyframe(CAPE_LEFT, glm::vec2(i * 0.0625f, 3 * 0.0625f));
	}

	sprite->setAnimationSpeed(MOVE_LEFT, 24);
	for (int i = 16; i >= 0; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i * 0.0625f, 4 * 0.0625f));
	}

	sprite->setAnimationSpeed(CAPE_RIGHT, 24);
	for (int i = 0; i < 16; ++i) {
		sprite->addKeyframe(CAPE_RIGHT, glm::vec2(i * 0.0625f, 1 * 0.0625f));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 24);
	for (int i = 0; i < 16; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i * 0.0625f, 2 * 0.0625f));
	}

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	for (int i = 1; i <= 4; ++i) {
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(1 - i * 0.0625f, 5 * 0.0625f));
	}

	sprite->setAnimationSpeed(FALL_LEFT, 18);
	for (int i = 1; i <= 9; ++i) {
		sprite->addKeyframe(FALL_LEFT, glm::vec2(1 - i * 0.0625f, 7 * 0.0625f));
	}
	for (int i = 0; i < 50; ++i) {
		sprite->addKeyframe(FALL_LEFT, glm::vec2(1 - 8 * 0.0625f, 7 * 0.0625f));
	}	// Cutre pero funciona

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	for (int i = 0; i < 4; ++i) {
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(i * 0.0625f, 5 * 0.0625f));
	}

	sprite->setAnimationSpeed(FALL_RIGHT, 18);
	for (int i = 0; i < 9; ++i) {
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(i * 0.0625f, 6 * 0.0625f));
	}
	for (int i = 0; i < 50; ++i) {
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(8 * 0.0625f, 6 * 0.0625f));
	}	// Cutre pero funciona

	sprite->setAnimationSpeed(ENTERING_DOOR, 6);
	for (int i = 0; i < 14; ++i) {
		sprite->addKeyframe(ENTERING_DOOR, glm::vec2(i * 0.0625f, 8 * 0.0625f));
	}

	sprite->setAnimationSpeed(DYING, 3);
	for (int i = 0; i < 15; ++i) {
		sprite->addKeyframe(DYING, glm::vec2(i < 9 ? i * 0.0625f : 9 * 0.0625f, 9 * 0.0625f));
	}	// Cutre pero funciona

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

int Player::update(int deltaTime)
{
	int i = 0;
	if (enteringDoor) {
		if (sprite->animation() != ENTERING_DOOR) {
			sprite->changeAnimation(ENTERING_DOOR);
			playerSound->stop();
		}
	}
	if (enteringDoor && !sprite->finalKeyframe()) {
		sprite->update(deltaTime);
	}
	else if (enteringDoor && sprite->finalKeyframe()) {
		alreadyEnteredDoor = true;
		playerSound->stop();
	}
	else if (dying) {
		if (sprite->animation() != DYING) {
			sprite->changeAnimation(DYING);
			playerSound->stop();
		}
		if (sprite->animation() == DYING && !sprite->finalKeyframe()) {
			sprite->update(deltaTime);
		}
		else if (sprite->animation() == DYING && sprite->finalKeyframe()) {
			dead = true;
			playerSound->stop();
		}
	}
	else {
		sprite->update(deltaTime);
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != CAPE_LEFT && sprite->animation() != MOVE_LEFT) {
			playerSound->play("sounds/steps.wav", true, false);
				startingMove = true;
			}
			if (startingMove) {
				if (sprite->animation() != CAPE_LEFT) {
					sprite->changeAnimation(CAPE_LEFT);
				}
				else if (sprite->finalKeyframe()) {
					startingMove = false;
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(glm::vec2(posPlayer.x, posPlayer.y+32), glm::ivec2(64, 32)))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
				playerSound->stop();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite->animation() != CAPE_RIGHT && sprite->animation() != MOVE_RIGHT) {
				startingMove = true;
			playerSound->play("sounds/steps.wav", true, false);
			}
			if (startingMove) {
				if (sprite->animation() != CAPE_RIGHT) {
					sprite->changeAnimation(CAPE_RIGHT);
				}
				else if (sprite->finalKeyframe()) {
					startingMove = false;
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			posPlayer.x += 2;
			if (map->collisionMoveRight(glm::vec2(posPlayer.x, posPlayer.y+32), glm::ivec2(64, 32)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
				playerSound->stop();
			}
		}
		else
		{
			playerSound->stop();
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == CAPE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == CAPE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y);
					if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == JUMP_LEFT)
						sprite->changeAnimation(FALL_LEFT);
					else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT)
						sprite->changeAnimation(FALL_RIGHT);
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y))
			{
				//como cambiar la tile
				i = map->changeTile(glm::ivec2(posPlayer.x + 16, posPlayer.y + FALL_STEP), glm::ivec2(24, 64), true);

				if (sprite->animation() == FALL_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == FALL_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
					if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
						sprite->changeAnimation(JUMP_LEFT);
					else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
				}
			}
			else {
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == JUMP_LEFT)
					sprite->changeAnimation(FALL_LEFT);
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT)
					sprite->changeAnimation(FALL_RIGHT);
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	return i;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosPlayer() {
	return posPlayer;
}

int Player::getHealth() {
	return health;
}

void Player::setHealth(int newHealth) {
	health = newHealth;
}

void Player::enterDoor(int deltaTime) {
	enteringDoor = true;
}

bool Player::enteredDoor() {
	return alreadyEnteredDoor;
}

void Player::die() {
	playerSound->stop();
	dying = true;
}

bool Player::alreadyDead() {
	return dead;
}