#include "Pickup.h"

#include <map>

#include "D3DFuncs.h"
#include "D3DManager.h"
#include "Mesh.h"
#include "Sound.h"
#include "Error.h"
#include "Player.h"
#include "Game.h"
#include "defines.h"

using namespace MSH;
using namespace VEC;
using namespace D3D;
using namespace std;
using namespace UDEF;


float Pickup::speed_ = 0.5f;
RandomNumberGenerator Pickup::rng_;

Pickup::Pickup() : GameObject(GameTypes::PICKUP), SPEED(0.35f), isAlive_(false)
{
}

Pickup::~Pickup()
{
	
}


void Pickup::Update( float elapsedSec )
{

	if (isAlive_){
		mPos.x -= Pickup::speed_;
		if(mPos.x<BORDER_LEFT){
			kill();
		}
	
		// checks for collision with player
		if (collidesWith(Game::Get()->GetPlayer().mPos, 3.0f)){
			Sound::Get()->PlayWaveFile(sfxHdl, false, 0.5f);
			kill();
			Game::Get()->GetPlayer().heal();
			Game::Get()->GetPlayer().getPickup();
			return;
		}
	}
}

void Pickup::spawn(bool randomizeX /*= false*/){
	// randomize scale?

	if (randomizeX) {
		mPos.x = rng_.getRandomValue(BORDER_RIGHT + 40) - 35;
	} else {
		mPos.x = BORDER_RIGHT;
	}
	mPos.y = rng_.getRandomValue(BORDER_TOP);
	isAlive_ = true;
}

void Pickup::reset(){
	isAlive_ = false;
}
void Pickup::Render()
{
	if (isAlive_){
	vector<D3DMATERIAL9> mtls;
	D3DMATERIAL9 mtl;
	CreateMaterial(1,1,1,mtl);
	mtls.insert(mtls.begin(), mpMesh->GetNumMaterials(),mtl);

	float scale=0.3f;
	D3DXMATRIX TranslateMat, ScaleMat, WorldMat, RotateMat;
	D3DXMatrixRotationX(&RotateMat, UDEF::D2R(180));
	RenderMesh(D3DManager::Get()->GetD3DDevice(), *mpMesh, &mtls[0], nullptr, mPos.x,mPos.y,0, scale,scale,scale, 0,1,0);
	}
}

void Pickup::Initialise(MSH::Mesh &myMesh)
{
	mpMesh=&myMesh;
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/explode1.wav",sfxHdl));
}

void Pickup::kill(){
	isAlive_ = false;
}

bool Pickup::collidesWith(const Vec2 &objPos, const float objRadius) const {
	Vec2 collPos;
	Vec2 collNorm;
	float mRad = 3.0f;

	bool hasCollided = CircleCircleCollision(objPos, objRadius, mPos, mRad, collPos, collNorm);

	return hasCollided;
}

void Pickup::PostRender()
{
}