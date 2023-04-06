#include "Scoreboard.h"
#include <glm/gtc/matrix_transform.hpp>

Scoreboard::Scoreboard(int nLifes, int score, int nStage) {
	currentStage = nStage;
	timeLeft = 60;
	points = score;
	playerLifes = nLifes;
	baseQuad = NULL;
	baseTexture = NULL;
}

Scoreboard::~Scoreboard() {
	if (baseQuad != NULL)
		delete baseQuad;
	if (baseTexture != NULL)
		delete baseTexture;
}

void Scoreboard::initScoreboard(ShaderProgram texProgram) {
	this->texProgram = texProgram;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 32.f) };
	glm::vec2 textCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	baseQuad = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	baseTexture = new Texture();
	baseTexture->loadFromFile("images/spritesheets/Scoreboard.png", TEXTURE_PIXEL_FORMAT_RGBA);

	geom[1] = glm::vec2(16.f, 16.f);
	for (int i = 0; i < 10; ++i) {
		numbers[i] = new Texture();
		numbers[i]->loadFromFile("images/spritesheets/numbers/" + to_string(i) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
		numbersQuads[i] = TexturedQuad::createTexturedQuad(geom, textCoords, texProgram);
	}

}

void Scoreboard::update(int deltaTime, double currentTime, int score, int lifesLeft) {
	timeLeft = 60 - int(currentTime);
	if (timeLeft <= 0) timeLeft = 0;//GAMEOVER();
	points = score;
	playerLifes = lifesLeft;
}

void Scoreboard::render() {
	baseQuad->render(*baseTexture);

	glm::mat4 modelview;

	// lifes
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(56.f, 8.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	int lifes = playerLifes == -1 ? 0 : playerLifes % 10;
	numbersQuads[lifes]->render(*numbers[lifes]);

	// score
	for (int i = 0; i < 6; ++i) {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f + i * 16.f, 8.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);

		int tmp = int(pow(10, (5 - i + 1))), tmp2 = int(pow(10, 5 - i));
		int index = points % tmp / tmp2;
		numbersQuads[index]->render(*numbers[index]);
	}

	// time left
	for (int i = 0; i < 2; ++i) {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(304.f + i * 16.f, 8.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);

		int tmp = int(pow(10, (1 - i + 1))), tmp2 = int(pow(10, 1 - i));
		int index = timeLeft % tmp / tmp2;
		numbersQuads[index]->render(*numbers[index]);
	}

	// stage
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(600.f, 8.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbersQuads[currentStage % 100 / 10]->render(*numbers[currentStage % 100 / 10]);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(616.f, 8.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	numbersQuads[currentStage % 10]->render(*numbers[currentStage % 10]);
}

int Scoreboard::getTimeLeft() {
	return timeLeft;
}

int Scoreboard::getScore() {
	return points;
}

bool Scoreboard::gameover() {
	return timeLeft <= 0;
}
