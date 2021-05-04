#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include <string>

#include "GameObject.h"
#include "Vector2.h"
#include "defines.h"
#include "ExplosionManager.h"
#include "Player.h"


namespace MSH { class Mesh; }

class EnemyBullet : public GameObject
{
public:
	EnemyBullet();
	~EnemyBullet();
	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise(MSH::Mesh &myMesh);

	void spawn(float, float, float);
	void remove();

	void reset();

	bool collidesWith(const Vec2 &objPos, const float objRadius, Vec2 &collPos, Vec2 &collNorm) const;

private:
	//all fighters share the one mesh
	MSH::Mesh *mpMesh;

	static float speed_;

	const float ENEMY_BULLET_SPEED;
};



#endif