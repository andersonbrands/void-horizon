#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <string>

#include "GameObject.h"
#include "Sprite.h"
#include "Vector2.h"



class WelcomeScreen
{
public:
	WelcomeScreen();
	~WelcomeScreen();

	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise();

private:

	Sprite *title_;
};


#endif