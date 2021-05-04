#ifndef EXPLOSION_MANAGER_H
#define EXPLOSION_MANAGER_H

#include "Vector2.h"
#include "GameObject.h"
#include "Explosion.h"
#include "RandomNumberGenerator.h"

#include "PoolAllocator.h"

class ExplosionManager : public PoolAllocator<Explosion>
{
public:
	ExplosionManager():PoolAllocator<Explosion>(20) {};
	~ExplosionManager();
	
	//load enemy models, create instances
	void Initialise();
	//tidy up on way out
	void Release();
	//move enemies and let them update
	void Update(float elapsedSec);
	//show all enemies
	void Render();
	//sprites, additive effects, etc
	void PostRender();
	void explode(float x, float y);

	void reset();

private:
	//int sfxHdl;
	static RandomNumberGenerator rng_;
};


#endif