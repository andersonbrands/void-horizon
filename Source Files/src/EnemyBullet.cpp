#include <map>

#include "EnemyBullet.h"
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

float EnemyBullet::speed_(1.0f);

EnemyBullet::~EnemyBullet()
{
}

EnemyBullet::EnemyBullet() : mpMesh(nullptr), GameObject(GameTypes::FIGHTER), ENEMY_BULLET_SPEED(0.5f)
{
}

void EnemyBullet::Update( float elapsedSec )
{
	if (GetActive()){
		mPos.x -= speed_;
		if(mPos.x<BORDER_LEFT)
			remove();

		Vec2 collPos;
		Vec2 collNorm;

		// checks for collision with player
		if (collidesWith(Game::Get()->GetPlayer().mPos, 3.25f, collPos, collNorm)){
			Game::Get()->GetPlayer().damage(10);
			Game::Get()->GetExplosionManager().explode(collPos.x, collPos.y);
			remove();
		}

	}
}

void EnemyBullet::Initialise( MSH::Mesh &myMesh )
{
	mpMesh=&myMesh;
}

void EnemyBullet::PostRender()
{
}

void EnemyBullet::Render()
{
	if (GetActive()){
		vector<D3DMATERIAL9> mtls;
		D3DMATERIAL9 mtl;
		CreateMaterial(1,0,0,mtl);
		mtls.insert(mtls.begin(),mpMesh->GetNumMaterials(),mtl);

		float scale=0.15f;
		RenderMesh(GetD3DManager()->GetD3DDevice(), *mpMesh, &mtls[0], nullptr, mPos.x, mPos.y, 0, scale,scale,scale, UDEF::g_PI/1,0,UDEF::g_PI/1);
	}
}

void EnemyBullet::spawn(float x, float y, float minSpeed){
	SetActive (true);

	EnemyBullet::speed_ = ENEMY_BULLET_SPEED + minSpeed * 1.05f;

	mPos.x = x;
	mPos.y = y;
}

void EnemyBullet::reset(){
	remove();
}

void EnemyBullet::remove(){
	SetActive (false);
}

bool EnemyBullet::collidesWith(const Vec2 &objPos, const float objRadius, Vec2 &collPos, Vec2 &collNorm) const {
	float mRad = 1.0f;

	bool hasCollided = CircleCircleCollision(objPos, objRadius, mPos, mRad, collPos, collNorm);

	return hasCollided;
}
