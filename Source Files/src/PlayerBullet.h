#ifndef PLAYER_BULLET_H
#define PLAYER_BULLET_H

#include <string>

#include "GameObject.h"
#include "Vector2.h"
#include "defines.h"


namespace MSH { class Mesh; }

class PlayerBullet : public GameObject
{
public:
	PlayerBullet();
	~PlayerBullet();
	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise(MSH::Mesh &myMesh);

	void reset();

	void spawn(float, float);
	void remove();

private:
	//all fighters share the one mesh
	MSH::Mesh *mpMesh;

	const float PLAYER_BULLET_SPEED;
};



#endif