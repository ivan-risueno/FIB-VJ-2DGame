#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "Vampire.h"


#define ENEMY_SPEED 1
#define FALL_STEP 1

enum EnemyAnims
{
	HIDE, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, FLY_RIGHT, FLY_LEFT,
};

//Cambiar el sprite y las animaciones
void Vampire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right, bool fly)
{
	enemySound = new Sound();
	movingRight = right;
	movingUp = true;
	floorTime = 0;
	downHits = 0;
	flying = fly;

	spritesheet.loadFromFile("images/spritesheets/VampireSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(HIDE, 8);
	sprite->addKeyframe(HIDE, glm::vec2(0.8f, 0.8f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.3125f, 0.125f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));


	sprite->setAnimationSpeed(MOVE_LEFT, 6);
	for (float i = 0.3125; i >= 0.0; i -= 0.0625) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i, 0.125f));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 6);
	for (float i = 0.25; i <= 0.5625; i += 0.0625) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i, 0.0f));
	}


	sprite->setAnimationSpeed(FLY_RIGHT, 6);
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.1875f, 0.25f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(FLY_LEFT, 6);
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.4375f, 0.25f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.3125f, 0.25f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.375f, 0.25f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.3125f, 0.25f));
	

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}


void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);

	//Behaviour when flying
	if (flying) {

		if (movingRight) {
			posEnemy.x += ENEMY_SPEED;
			if (sprite->animation() != FLY_RIGHT) {
				sprite->changeAnimation(FLY_RIGHT);
			}
			if (map->collisionMoveRightEnemy(glm::vec2(posEnemy.x, posEnemy.y), glm::ivec2(64, 64))) {
				movingRight = false;
				posEnemy.x -= ENEMY_SPEED * 4;
			}

		}
		else if (!movingRight) {
			posEnemy.x -= ENEMY_SPEED;
			if (sprite->animation() != FLY_LEFT) {
				sprite->changeAnimation(FLY_LEFT);
			}
			if (map->collisionMoveLeftEnemy(glm::vec2(posEnemy.x, posEnemy.y), glm::ivec2(64, 64))) {
				movingRight = true;
				posEnemy.x -= ENEMY_SPEED * 4;
			}
		}
		if (movingUp) {
			posEnemy.y -= ENEMY_SPEED;
			if (map->collisionMoveUp(glm::vec2(posEnemy.x + 21, posEnemy.y), glm::ivec2(30, 64))) {
				movingUp = false;
				posEnemy.y += ENEMY_SPEED * 2;
			}
			

		}
		else if (!movingUp) {
			posEnemy.y += ENEMY_SPEED;
			if (map->collisionMoveDown(glm::vec2(posEnemy.x+21, posEnemy.y), glm::ivec2(30, 64))) {
				movingUp = true;
				posEnemy.y -= ENEMY_SPEED;
				++downHits;
			}
			if (downHits == maxDownHits) {
				flying = false;
				downHits = 0;
			}
		}
	}


	//Behaviour when not flying
	else if (!flying) {
		if (movingRight) {
			posEnemy.x += ENEMY_SPEED;
			if (sprite->animation() != MOVE_RIGHT) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
			if (map->collisionMoveRightEnemy(glm::vec2(posEnemy.x +21, posEnemy.y+21), glm::ivec2(30, 42))) {
				movingRight = false;
				posEnemy.x -= ENEMY_SPEED * 2;
			}
			else if (!map->collisionMoveDown(glm::vec2(posEnemy.x+42, posEnemy.y + FALL_STEP), glm::ivec2(30, 64))) {
				movingRight = false;
				posEnemy.x -= ENEMY_SPEED * 2;
			}

		}

		else if (!movingRight) {
			posEnemy.x -= ENEMY_SPEED;
			if (sprite->animation() != MOVE_LEFT) {
				sprite->changeAnimation(MOVE_LEFT);
			}
			if (map->collisionMoveLeftEnemy(glm::vec2(posEnemy.x+21, posEnemy.y+21), glm::ivec2(21, 42))) {
				movingRight = true;
				posEnemy.x -= ENEMY_SPEED * 2;
			}
			
			else if (!map->collisionMoveDown(glm::vec2(posEnemy.x, posEnemy.y + FALL_STEP), glm::ivec2(21, 64))) {
				movingRight = true;
				posEnemy.x += ENEMY_SPEED * 2;
			}
		}

		++floorTime;
		if (floorTime >= maxFloortime && !map->collisionMoveDown(glm::vec2(posEnemy.x+21, posEnemy.y), glm::ivec2(30, 64))
			&& !map->collisionMoveUp(glm::vec2(posEnemy.x+21, posEnemy.y), glm::ivec2(30, 64))) {
			floorTime = 0;
			flying = true;
			enemySound->play("sounds/bats.wav", false);
		}
	}

	

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}