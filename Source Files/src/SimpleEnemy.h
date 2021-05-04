#ifndef SIMPLE_ENEMY_H
#define SIMPLE_ENEMY_H

#include <string>

#include "GameObject.h"
#include "Vector2.h"
#include "defines.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "ExplosionManager.h"
#include "EnemyBulletManager.h"

namespace MSH { class Mesh; }
using namespace VEC;


class SimpleEnemy : public GameObject
{
public:
	SimpleEnemy();
	~SimpleEnemy();
	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise(MSH::Mesh &myMesh);

	void spawn(float, float);

	void reset();

	bool isAlive() const { return isAlive_; };

	bool isAlive_;
private:
	//all fighters share the one mesh
	MSH::Mesh *mpMesh;

	static RandomNumberGenerator rng_;

	bool willShoot_;

	static float speed_;
	static int killCount;

	int sfxHdl;
	void kill();

	bool collidesWith(const Vec2 &objPos, const float objRadius, Vec2 &collPos, Vec2 &collNorm) const;
};



#endif