// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <string>
#include "Vector2.h"
#include "Sprite.h"
#include "GameObject.h"

using namespace VEC;

/*
We like things to go bang a lot so it makes sense ot have
a reusable class. It plays an animated texture explosion
and creates a force that damages and blows things away.
Also lights up the scene.
*/
class Explosion : public GameObject
{
public:
	Explosion();
	~Explosion();
	//we reuse things, no run time allocation
	void Init();
	//keep the anim playing
	virtual void Update(float aTime);

	virtual void Render();
	//transparent additive effect so needs rendering after everything else
	virtual void PostRender();
	//is it still exploding?
	bool GetIsPlaying() const;
	//how many times has the explosion played completely
	int GetNumPlays() const;
	//start the animation playing at a specific position, lit or unlit
	void Play(const Vec2 &pos, bool lit);
	//set how big th eexplosion is (radius)
	void SetSize(float sz);
	float GetSize() const;

	void reset();

private:
	Sprite *mpSpr;			//the texture animation
	//uint32 mLightHdl;	    //the light we'll use
	float mTimer;			//used to help us expand and then contract the explosion's light
	float mSize;			//how big is the explosion
};

#endif