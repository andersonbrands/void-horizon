#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Sprite;

class Bullet : public GameObject
{
public:
	static int CalcType() { return GameObject::GetTypeFromName("BULLET"); }
	Bullet();
	virtual ~Bullet();	//we've got an abstract base so need a virtual destructor

	// Virtual methods from GameObject class
	virtual void Update(float aTime);
	virtual void OnCollide(const GameObject* apOther);
	virtual void Render() {}
	virtual void PostRender();
	virtual GameObject *Create() { return new Bullet; }
	//virtual bool GetResultantMomentum(Vector2 &vel, GameObject *pCollider) { return false; }
	virtual void Init();
	//virtual void AddForce(const Vector2 &force, float damage) {}

	//we don't want bullets blowing up the ship as the leave, so
	//get the type of the ship so we can ignore it
	//void SetSpawnerType(UDEF::uint32 type) { mSpawnerType=type; }

private:

	void Release();

	Sprite *mpSprite;			//bullets appear as a small sprite
	float mLife;				//bullets don't live very long
	//UDEF::uint32 mSpawnerType;  //remember where we came from
	//UDEF::uint32 mLightHdl;		//bullets carry a point light with them
};




#endif