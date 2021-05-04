#ifndef STAR_H
#define STAR_H

#include <string>

#include "GameObject.h"
#include "Vector2.h"
#include "defines.h"
#include "RandomNumberGenerator.h"

namespace MSH { class Mesh; }


class Star : public GameObject
{
public:
	Star();
	~Star();

	virtual void Render();
	virtual void PostRender();
	virtual void Update(float elapsedSec);

	void Initialise(MSH::Mesh &myMesh);

	void spawn(bool randomizeX = false);

private:
	static RandomNumberGenerator rng_;
	MSH::Mesh *mpMesh;

	float scale;
	//float scaleX;
	//float scaleY;

	float rotation;
	float rotationGain;
	const float SPEED;
};


#endif