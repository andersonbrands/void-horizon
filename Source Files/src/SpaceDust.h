#ifndef SPACE_DUST_H
#define SPACE_DUST_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "Star.h"

#include "PoolAllocator.h"

//#define TOTAL_STARS 190

class SpaceDust : public PoolAllocator<Star>
{
public:
	SpaceDust(): PoolAllocator<Star>(190) {};
	~SpaceDust();
	
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

private:
	//instances share the same mesh
	MSH::Mesh meshStar_;
};


#endif