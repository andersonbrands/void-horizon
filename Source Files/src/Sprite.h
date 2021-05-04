// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef SPRTIE_H
#define SPRTIE_H

#include <vector>
#include <string>
#include <d3d9.h>
#include <d3dx9math.h>
#include "defines.h"
#include "Singleton.h"
#include "Vector2.h"
#include <string>

using namespace VEC;

namespace RenderFlags { typedef enum {
	ALPHA_BLEND_ENABLE=1,		//need this if you want blending at all
	ALPHA_BLEND_ADDITIVE=2,		//brighten up the lighter parts of the texture with the screen
	ALPHA_BLEND_INVSOURCE=4,	//make things transparent
	MODULATEX2=8,				//make thigns glow
	MODULATEX4=16,				//make thigns glow a lot
	NO_Z_BUFFER=32				//ignore the z buffer, good for overlays
}Enum;}

//convenience struct to isolate part of a texture when animating
struct UVPair	
{ 
	UVPair(float u0, float v0, float u1, float v1) : mUVTL(u0,v0), mUVBR(u1,v1) {}
	bool operator==(const UVPair &rhs) {
		return mUVTL==rhs.mUVTL && mUVBR==rhs.mUVBR;
	}
	Vec2 mUVTL, mUVBR; 
};
//for convenience when comparing
inline bool operator==(const UVPair &lhs, const UVPair &rhs)
{
	return lhs.mUVBR.x==rhs.mUVBR.x && lhs.mUVTL.x==rhs.mUVTL.x &&
		lhs.mUVBR.y==rhs.mUVBR.y && lhs.mUVTL.y==rhs.mUVTL.y;
}

class SpriteAnim;

/*
A sprite is a 2D camera facing image, either texture animated or not
*/
class Sprite
{
friend class SpriteManager;
public:

	Vec2 mPos;		//position - centre of top left corner depending on flags
	Vec2 mWorldSize;	//how big is it in the world
	float mZ;			//as this is part of the world we can specify a Z value to order it
	D3DXCOLOR mColour;	
	unsigned int mFlags;//alpha blend, additive, etc
	bool mCentred;		
	float mDegrees;		//it rotating - either about its centre of top left corner

	Sprite();
	~Sprite();
	//uses the sprite manager to load in its texture
	void Load(const std::string &fileName); 
	void Render();
	//if its animating then it needs a chance to update
	void Update(float aTime);

	//nobody needs to know about the separate animation object inside, it's separate to save memory
	//when not needed. We add it's functions to the main sprite interface and pass them through so users
	//don't need to know that implementation detail
	void ReserveFrames(int nFrames);
	void Play();
	void Stop();
	const UVPair &GetTexCoords() const;
	void AddFrames(UVPair &uvs);
	void AddFrames(float u0, float v0, float u1, float v1);
	void AddMultipleFrames(int nFrames, float tw, float th);
	bool GetIsPlaying() const;
	int GetNumPlays() const;
	void SetAnim(float frameRate, bool loop, bool play, int numPlays);
	void SetNumPlays(int num);
	float GetFrameRate() const;
	int GetNumFrames() const;

private:

	unsigned int mSprHdl;	//sprites load resources through the manager and the get a unique handle
	SpriteAnim *mpAnim;		//animation data is kept in here if needed

	//when needed we'll allocate animation data
	void CheckAnim();
};

class SpriteData;

/*
Sprites are used all over the game
We don't want to load multiple copies of the same textures
*/
class SpriteManager : public Singleton<SpriteManager>
{
public:
	SpriteManager();
	~SpriteManager();
	//the sprite describes how it should be renderer so pass it to 
	//the manager when you want it rendering. Eventually this should
	//shift to caching the sprite into a batch for efficient rendering
	void Render(Sprite &spr);
	
	//load a new texture and the sprite it goes with
	unsigned int Load(const std::string &texture, Sprite &spr);

private:

	LPDIRECT3DVERTEXBUFFER9 mpVerts;	//a quad vertex buffer that all simple sprites use without modification
	D3DMATERIAL9 mMtl;		//set the colour

	//array of texture/vertex data
	typedef std::vector<SpriteData*> Chunks;
	Chunks mChunks;

	void SetupMaterial(float r, float g, float b);
};



SINGLETON_GET(SpriteManager);


#endif