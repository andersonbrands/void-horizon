#ifndef PLAYER_H
#define PLAYER_H

#include "defines.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Vector2.h"
#include "PlayerBulletManager.h"

class Sprite;
class Explosion;

/*
Player controlled ship moves with cursor keys
*/
class Player : public GameObject
{
public:
	Player();
	~Player();
	void Initialise();
	void Release();

	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	const std::vector<PlayerBullet*> getPlayerBullets(){ return playerBulletManager_.getPlayerBullets(); };

	void damage(int);
	void heal();
	void kill();

	void reset();

	void destroyEnemy();
	void missEnemy();

	void get();
	void getPickup();
	void missPickup();

	int hp_;
	int score_;
	int destroyedEnemiesCount_;
	int missedEnemiesCount_;

	int destroyedPickupsCount_;
	int missedPickupsCount_;

private:
	void spawn(float x, float y);
	MSH::Mesh mMesh;
	PlayerBulletManager playerBulletManager_; 

	float maxSpeed_;
	VEC::Vec2 speed_;
	VEC::Vec2 rotation_;


	const float FRICTION;
	const float SPEED_STEP;
};


#endif