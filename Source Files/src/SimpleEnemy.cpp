#include <map>

#include "SimpleEnemy.h"
#include "D3DFuncs.h"
#include "D3DManager.h"
#include "Mesh.h"
#include "Sound.h"
#include "Error.h"
#include "Player.h"
#include "Game.h"
#include "defines.h"
#include "PlayerBullet.h"

using namespace MSH;
using namespace VEC;
using namespace D3D;
using namespace std;
using namespace UDEF;


RandomNumberGenerator SimpleEnemy::rng_;

float SimpleEnemy::speed_ = 0.6f;
int SimpleEnemy::killCount = 0;

SimpleEnemy::~SimpleEnemy()
{
	
}

SimpleEnemy::SimpleEnemy() : mpMesh(nullptr), GameObject(GameTypes::FIGHTER), willShoot_(false)
{

}

void SimpleEnemy::Update( float elapsedSec )
{
	if (isAlive_){
		mPos.x -= SimpleEnemy::speed_;
		if(mPos.x<BORDER_LEFT){
			kill();
			Game::Get()->GetPlayer().missEnemy();
		}

		Vec2 collPos;
		Vec2 collNorm;

		// checks for collision with player
		if (collidesWith(Game::Get()->GetPlayer().mPos, 3.25f, collPos, collNorm)){
			Sound::Get()->PlayWaveFile(sfxHdl, false, 0.5f);
			kill();
			++killCount;
			Game::Get()->GetPlayer().damage(20);
			Game::Get()->GetPlayer().destroyEnemy();
			Game::Get()->GetExplosionManager().explode(collPos.x, collPos.y);
			return;
		}

		std::vector<PlayerBullet*> playerBullets = Game::Get()->GetPlayer().getPlayerBullets();
		// checks for collision with player bullets
		for (int i(0); i < playerBullets.size(); ++i){
			if(playerBullets[i]->GetActive() && collidesWith(playerBullets[i]->mPos, 1.0f, collPos, collNorm)){
				playerBullets[i]->remove();
				++killCount;
				Game::Get()->GetPlayer().destroyEnemy();
				Game::Get()->GetExplosionManager().explode(collPos.x, collPos.y);

				Sound::Get()->PlayWaveFile(sfxHdl, false, 0.5f);
				kill();
				return;
			}
		}

		if (willShoot_ && rng_.getRandomValue(100) == 5) {
			Game::Get()->GetEnemyBulletManager().spawnBullet(mPos.x, mPos.y, SimpleEnemy::speed_);
			willShoot_ = false;
		}

		if (killCount == 4){
			killCount = 0;
			speed_ += 0.1f;
		}
	}
}

void SimpleEnemy::Initialise( MSH::Mesh &myMesh )
{
	mpMesh=&myMesh;
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/Explode2.wav",sfxHdl));
}

void SimpleEnemy::PostRender()
{
}

void SimpleEnemy::Render()
{
	if (isAlive_){
		vector<D3DMATERIAL9> mtls;
		D3DMATERIAL9 mtl;
		CreateMaterial(1,1,1,mtl);
		mtls.insert(mtls.begin(),mpMesh->GetNumMaterials(),mtl);

		float scale=0.3f;
		RenderMesh(GetD3DManager()->GetD3DDevice(), *mpMesh, &mtls[0], nullptr, mPos.x, mPos.y, 0, scale,scale,scale, UDEF::g_PI/2,UDEF::g_PI,UDEF::g_PI/2);
	}
}

void SimpleEnemy::reset(){
	isAlive_ = false;
	speed_ = 0.6f;
}

void SimpleEnemy::spawn(float x, float y){
	isAlive_ = true;

	willShoot_ = true;

	mPos.x = x - 15.0f;
	mPos.y = y;
}

void SimpleEnemy::kill(){
	isAlive_ = false;
}

bool SimpleEnemy::collidesWith(const Vec2 &objPos, const float objRadius, Vec2 &collPos, Vec2 &collNorm) const {
	float mRad = 3.25f;

	Vec2 enemyOffset(7.0f + mPos.x, 0.0f + mPos.y);
	bool hasCollided = CircleCircleCollision(objPos, objRadius, enemyOffset, mRad, collPos, collNorm);

	return hasCollided;
}
