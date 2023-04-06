#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Texture.h"
#include "Sprite.h"
#include "Sound.h"


class MainMenu
{

public:
	MainMenu(ShaderProgram texProgram);
	~MainMenu();

	void initSplash();
	void initMenu();
	void update(int deltaTime);

	void selectPlay();
	void selectInstructions();
	void selectCredits();
	void selectExit();
	void render();
	bool startPlaying() const;
	bool Splash() const;
	void showCredits();

private:
	bool splash, keyPressed, play;
	int currentTime;
	int optionSelected;
	ShaderProgram texProgram;

	TexturedQuad* screenQuad, *extraQuad;
	Texture* screenTexture, *extraTexture;
	Texture* selectorTexture;
	Sprite* selector;
	Sound* menuSound;
	bool showingInstructions, showingCredits;
};

#endif //_MENU_INCLUDE