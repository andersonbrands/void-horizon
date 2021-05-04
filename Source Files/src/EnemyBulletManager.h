#ifndef ENEMY_BULLET_MANAGER_H
#define ENEMY_BULLET_MANAGER_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "EnemyBullet.h"
#include "RandomNumberGenerator.h"

#include "PoolAllocator.h"

class EnemyBulletManager : public PoolAllocator<EnemyBullet>
{
public:
	EnemyBulletManager(Player& p, ExplosionManager& em): PoolAllocator<EnemyBullet>(20) {};
	~EnemyBulletManager();
	
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
	void spawnBullet(float x, float y, float minSpeed, int numBullets = 1);

	void reset();

	const std::vector<EnemyBullet*> getEnemyBullets() { return objects_; };

private:
	//instances share the same mesh
	MSH::Mesh meshEnemyBullet_;

	int sfxHdl;
	static RandomNumberGenerator rng_;
};


#endif