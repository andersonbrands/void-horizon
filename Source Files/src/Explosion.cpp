// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "Explosion.h"
#include "Game.h"
#include "D3DManager.h"


Explosion::Explosion() : GameObject(GameTypes::EXPLOSION), mTimer(0), mSize(0)
{
	//done once at the start
	mpSpr=new Sprite;
	//the explosion texture has 23 frames of animation on it
	mpSpr->AddMultipleFrames(23, 103.f/512.f, 103.f/512.f);
	mpSpr->Load("bin/data/explosion_opaque.dds");
	Init();
}

void Explosion::Init()
{
	//reset everything
	mpSpr->mCentred=true;
	mpSpr->mZ = 0;
	mpSpr->SetAnim(0.05f, false, false,0);
	mpSpr->mFlags=RenderFlags::ALPHA_BLEND_ADDITIVE|RenderFlags::ALPHA_BLEND_ENABLE|RenderFlags::NO_Z_BUFFER;
	mpSpr->SetNumPlays(0);

	SetSize(10);
}

Explosion::~Explosion()
{
	delete mpSpr;
}

bool Explosion::GetIsPlaying() const
{
	return mpSpr->GetIsPlaying();
}

int Explosion::GetNumPlays() const
{
	return mpSpr->GetNumPlays();
}

void Explosion::SetSize( float sz )
{
	//scale up or down the explosion texture
	MPOD_ASSERT(sz>0);
	mpSpr->mWorldSize=Vec2(sz,sz);
}

void Explosion::Play(const Vec2 &pos, bool lit)
{
	SetActive(true);
	//start it animating
	mpSpr->mPos=pos;
	mpSpr->mDegrees=(float)(rand()%360);
	mpSpr->Play();

	mTimer=0;
}

void Explosion::Update( float aTime )
{
	if (!GetActive()) {
		return;
	}
	mpSpr->Update(aTime);
	
	if(mpSpr->GetIsPlaying())
	{
		//we want the explosion's light to expand rapidly and then contract
		mTimer+=aTime;
		float fadeUp=mpSpr->GetFrameRate()*mpSpr->GetNumFrames();
		float fadeOut(fadeUp), full(fadeUp);
		fadeUp*=0.2f;
		fadeOut*=0.3f;
		mSize=mpSpr->mWorldSize.x*1.5f;
		if(mTimer<fadeUp)
			mSize*=mTimer/fadeUp;
		else if(mTimer>(full-fadeOut))
			mSize*=1.f - (mTimer-(full-fadeOut))/fadeOut;
	} else {
		SetActive(false);
	}
}

void Explosion::Render() {
	
}

void Explosion::reset() {
	SetActive(false);
}

void Explosion::PostRender()
{
	if(!mpSpr->GetIsPlaying())
		return;
	if (GetActive())
		mpSpr->Render();
}

float Explosion::GetSize() const
{
	return mpSpr->mWorldSize.x;
}
