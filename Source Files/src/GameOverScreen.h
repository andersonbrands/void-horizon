#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER__SCREEN_H

#include <string>

#include "GameObject.h"
#include "Sprite.h"

class GameOverScreen
{
public:
	GameOverScreen();
	~GameOverScreen();

	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise();

private:

	Sprite *title_;
	Sprite *gameOver_;
};


#endif