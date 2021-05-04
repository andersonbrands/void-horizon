#ifndef PICKUP_H
#define PICKUP_H

#include <string>

#include "GameObject.h"
#include "Vector2.h"
#include "defines.h"
#include "RandomNumberGenerator.h"
#include "Player.h"

namespace MSH { class Mesh; }
using namespace VEC;


class Pickup : public GameObject
{
public:
	Pickup();
	~Pickup();

	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise(MSH::Mesh &myMesh);

	void spawn(bool randomizeX = false);

	bool isAlive() const { return isAlive_; };

	void reset();

	bool isAlive_;

private:
	static RandomNumberGenerator rng_;
	MSH::Mesh *mpMesh;

	const float SPEED;
	static float speed_;

	int sfxHdl;
	void kill();

	bool collidesWith(const Vec2 &objPos, const float objRadius) const;
};


#endif