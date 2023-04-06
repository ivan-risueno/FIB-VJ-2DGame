#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "Skeleton.h"


#define ENEMY_SPEED 1
#define FALL_STEP 1

enum EnemyAnims
{
	START_RIGHT, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_LEFT
};

//Cambiar el sprite y las animaciones
void Skeleton::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool right)
{
	enemySound = new Sound();
	movingRight = right;
	startingMove = true;

	spritesheet.loadFromFile("images/spritesheets/SkeletonSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.125f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(START_LEFT, 6);
	for (float i = 0.4375; i >= 0.0; i -= 0.0625) {
		sprite->addKeyframe(START_LEFT, glm::vec2(i, 0.1875f));
	}

	sprite->setAnimationSpeed(START_RIGHT, 6);
	for (float i = 0.0; i <= 0.4375; i += 0.0625) {
		sprite->addKeyframe(START_RIGHT, glm::vec2(i, 0.0625f));
	}

	sprite->setAnimationSpeed(MOVE_LEFT, 6);
	for (float i = 0.25; i >= 0.0; i -= 0.0625) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i, 0.125f));
	}
	for (float i = 0.0625; i <= 0.1875; i += 0.0625) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i, 0.125f));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 6);
	for (float i = 0.0; i <= 0.25; i += 0.0625) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i, 0.0f));
	}
	for (float i = 0.1875; i >= 0.0625; i -= 0.0625) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i, 0.0f));
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}


void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (movingRight) {
		posEnemy.x += ENEMY_SPEED;
		if (startingMove) {
			if (sprite->animation() != START_RIGHT) {
				sprite->changeAnimation(START_RIGHT);
			}
			else if (sprite->finalKeyframe()) {
				sprite->changeAnimation(MOVE_RIGHT);
				startingMove = false;
			}
		}
		else if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
			enemySound->play("sounds/bonesRight.wav", false);
		}
		if (map->collisionMoveRightEnemy(glm::vec2(posEnemy.x+21, posEnemy.y+21), glm::ivec2(21, 42))) {
			movingRight = false;
			posEnemy.x -= ENEMY_SPEED * 2;
		}
		else if (!map->collisionMoveDown(glm::vec2(posEnemy.x + 48, posEnemy.y + FALL_STEP), glm::ivec2(64, 64))) {
			movingRight = false;
			posEnemy.x -= ENEMY_SPEED * 2;
		}
		
	}

	else if (!movingRight) {
		posEnemy.x -= ENEMY_SPEED;
		if (startingMove) {
			if (sprite->animation() != START_LEFT) {
				sprite->changeAnimation(START_LEFT);
			}
			else if (sprite->finalKeyframe()) {
				sprite->changeAnimation(MOVE_LEFT);
				startingMove = false;
			}
		}
		else if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
			enemySound->play("sounds/bonesLeft.wav", false);
		}
		if (map->collisionMoveLeftEnemy(glm::vec2(posEnemy.x+21, posEnemy.y+21), glm::ivec2(21, 42))) {
			movingRight = true;
			posEnemy.x -= ENEMY_SPEED * 2;
		}
		else if (!map->collisionMoveDown(glm::vec2(posEnemy.x - 48, posEnemy.y + FALL_STEP), glm::ivec2(64, 64))) {
			movingRight = true;
			posEnemy.x += ENEMY_SPEED * 2;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}