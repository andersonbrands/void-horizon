#ifndef PLAYER_BULLET_MANAGER_H
#define PLAYER_BULLET_MANAGER_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "PlayerBullet.h"
#include "RandomNumberGenerator.h"

#include "PoolAllocator.h"

//#define TOTAL_BULLETS 20

class PlayerBulletManager : public PoolAllocator<PlayerBullet>
{
public:
	PlayerBulletManager():PoolAllocator<PlayerBullet>(20) {};
	~PlayerBulletManager();
	
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
	void spawnBullet(float x, float y, int numBullets = 1);
	void reset();

	const std::vector<PlayerBullet*> getPlayerBullets() { return objects_; };

private:
	//instances share the same mesh
	MSH::Mesh meshPlayerBullet_;

	int sfxHdl;
	static RandomNumberGenerator rng_;
};


#endif