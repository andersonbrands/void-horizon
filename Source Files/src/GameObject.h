#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2.h"

/*
If all game objects (player, enemies) inherit from this then
they can share common behavior and any game object can be kept 
in one container (array) so you can iterate over them all at once if you want
*/
class GameObject
{
public:
	//give a unique type to any game object
	typedef unsigned int ObjType;
	static const ObjType NULL_TYPE=0xffffffff;
	static int GetTypeFromName(const char *pType);
	GameObject(ObjType type) : mType(type), mActive(false) {}
	virtual ~GameObject() {}
	//normal rendering
	virtual void Render()=0;
	//anything that needs rendering last like engine sprites or additive effects
	virtual void PostRender() {}
	//run logic and move
	virtual void Update(float elapsedSec)=0;

	//does this object need rendering and updating
	bool GetActive() const {
		return mActive; }
	void SetActive(bool active) {
		mActive=active; }
	//what type of object is this
	unsigned int GetType() const {
		return mType; }

	//world position, no need for Z if the game is 2D
	VEC::Vec2 mPos;

	void SetVelocityLimit(float limit)
	{
		//MPOD_ASSERT_MSG(limit>=0 && limit<9999,"Bad velocity limit");
		mVelLimit=limit;
	}

private:
	//what is this object?
	ObjType mType;
	//is it updating+rendering
	bool mActive;
	float mVelLimit;	//hard limit

};



#endif