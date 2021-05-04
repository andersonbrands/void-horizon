#ifndef PICKUP_MANAGER_H
#define PICKUP_MANAGER_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "Pickup.h"
#include "RandomNumberGenerator.h"
#include "Player.h"

#include "PoolAllocator.h"


class PickupManager : public PoolAllocator<Pickup>
{
public:
	PickupManager():PoolAllocator<Pickup>(1), MAX_PICKUPS_PER_WAVE(1) {};
	~PickupManager();
	
	//load enemy models, create instances
	virtual void Initialise();
	//tidy up on way out
	void Release();
	//move enemies and let them update
	void Update(float elapsedSec);
	//show all enemies
	void Render();
	//sprites, additive effects, etc
	void PostRender();

	void reset();

private:
	//instances share the same mesh
	MSH::Mesh meshPickup_;

	static RandomNumberGenerator rng_;

	void sendWave(int numPickups);
	const int MAX_PICKUPS_PER_WAVE;
};


#endif