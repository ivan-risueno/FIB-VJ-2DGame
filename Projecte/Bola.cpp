#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "Bola.h"


#define ENEMY_SPEED 1
#define FALL_STEP 1

enum EnemyAnims
{
	HIDE, SPAWN, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_LEFT, START_RIGHT, COLLIDE, FALL, MELT
};

//Cambiar el sprite y las animaciones
void Bola::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right)
{
	enemySound = new Sound();
	movingRight = right;
	melting = false;
	startingMove = false;
	spawning = true;
	colliding = false;

	spritesheet.loadFromFile("images/spritesheets/BolaSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);

	sprite->setAnimationSpeed(HIDE, 8);
	sprite->addKeyframe(HIDE, glm::vec2(0, 0.5f));

	sprite->setAnimationSpeed(SPAWN, 3);
	for (float i = 0.125; i >= 0; i -= 0.0625) {
		sprite->addKeyframe(SPAWN, glm::vec2(i, 0.125f));
	}

	sprite->setAnimationSpeed(START_RIGHT, 8);
	for (float i = 0.f; i <= 0.25; i += 0.0625) {
		sprite->addKeyframe(START_RIGHT, glm::vec2(i, 0.f));
	}

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(START_LEFT, 8);
	for (float i = 0.5625; i >= 0.3125; i -= 0.0625) {
		sprite->addKeyframe(START_LEFT, glm::vec2(i, 0.f));
	}

	sprite->setAnimationSpeed(MOVE_LEFT, 5);
	for (float i = 0.375; i >= 0.3125; i -= 0.0625) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i, 0.f));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 5);
	for (float i = 0.1875; i <= 0.25; i += 0.0625) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i, 0.f));
	}

	sprite->setAnimationSpeed(COLLIDE, 8);
	for (float i = 0.0625; i <= 0.1875; i += 0.0625) {
		sprite->addKeyframe(COLLIDE, glm::vec2(i, 0.0625f));
	}

	sprite->setAnimationSpeed(FALL, 8);
	sprite->addKeyframe(FALL, glm::vec2(0.0625f, 0.0625f));
	for (float i = 0; i <= 0.1875; i += 0.0625) {
		sprite->addKeyframe(FALL, glm::vec2(i, 0.125f));
	}

	sprite->setAnimationSpeed(MELT, 3);
	for (float i = 0; i <= 0.125; i += 0.0625) {
		sprite->addKeyframe(MELT, glm::vec2(i, 0.125f));
	}
	for (int i = 0; i < 10; ++i) {
		sprite->addKeyframe(MELT, glm::vec2(0, 0.5f));
	}


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}


int Bola::update(int deltaTime)
{
	int i = 0;
	sprite->update(deltaTime);

	if (melting || spawning) {
		if (melting) {
			if (sprite->animation() != MELT) {
				sprite->changeAnimation(MELT);
				enemySound->play("sounds/melt.wav", false);
			}
			if (sprite->finalKeyframe()) {
				melting = false;
				spawning = true;
				posEnemy.x = posBola.x;
				posEnemy.y = posBola.y;
			}
		}

		if (spawning) {
			if (sprite->animation() != SPAWN) {
				sprite->changeAnimation(SPAWN);
				enemySound->play("sounds/spawn.wav", false);
			}
			if (sprite->finalKeyframe()) {
				spawning = false;
				startingMove = true;
				if (movingRight) {
					sprite->changeAnimation(START_RIGHT);
				}
				else {
					sprite->changeAnimation(START_LEFT);
				}
			}
		}
	}

	else {
		if (colliding) {
			if (sprite->animation() != COLLIDE) {
				sprite->changeAnimation(COLLIDE);
			}
			else if (sprite->finalKeyframe()) {
				colliding = false;
			}

		}

		if (movingRight && !colliding) {
			posEnemy.x += ENEMY_SPEED;
			if ((startingMove)) {
				if (sprite->animation() != START_RIGHT) {
					sprite->changeAnimation(START_RIGHT);
				}
				else if (sprite->finalKeyframe()) {
					sprite->changeAnimation(MOVE_RIGHT);
					startingMove = false;
				}
			}
			if (map->collisionMoveRight(glm::ivec2(posEnemy.x + 32, posEnemy.y), glm::ivec2(32, 32))) {
				movingRight = false;
				posEnemy.x -= ENEMY_SPEED * 2;
				startingMove = true;
				colliding = true;
			}

		}

		else if (!movingRight && !colliding) {
			posEnemy.x -= ENEMY_SPEED;
			if ((startingMove)) {
				if (sprite->animation() != START_LEFT) {
					sprite->changeAnimation(START_LEFT);
				}
				else if (sprite->finalKeyframe()) {
					sprite->changeAnimation(MOVE_LEFT);
					startingMove = false;
				}
			}
			if (map->collisionMoveLeft(glm::ivec2(posEnemy.x, posEnemy.y), glm::ivec2(32, 32))) {
				movingRight = true;
				posEnemy.x += ENEMY_SPEED * 2;
				startingMove = true;
				colliding = true;
			}
		}
	}

	if (posEnemy.x == posMelt.x && posEnemy.y == posMelt.y) {
		melting = true;
	}

	posEnemy.y += FALL_STEP;

	if (map->collisionMoveDown(glm::ivec2(posEnemy.x + 16,posEnemy.y), glm::ivec2(32, 64), &posEnemy.y)) {
		i = map->changeTile(glm::ivec2(posEnemy.x+10, posEnemy.y + FALL_STEP), glm::ivec2(24, 64), false);
	}
		
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

	return i;

}

void Bola::setPosition(const glm::vec2& pos, const glm::vec2& posM)
{
	posMelt = posM;
	posBola = pos;
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}