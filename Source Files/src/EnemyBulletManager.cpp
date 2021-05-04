#include "EnemyBulletManager.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "Utils.h"
#include "GameObject.h"
#include "Player.h"
#include "Sound.h"
#include "Game.h"
#include "defines.h"

using namespace MSH;
using namespace D3D;
using namespace VEC;
using namespace std;
using namespace UDEF;


RandomNumberGenerator EnemyBulletManager::rng_;

void EnemyBulletManager::Initialise(){
	meshEnemyBullet_.InitialiseMesh(GetD3DManager()->GetD3DDevice(), "bin/data/", "bullet.x");

	for(int i=0;i<totalObjects_;++i){
		objects_[i]->Initialise(meshEnemyBullet_);
	}

	//sound effect
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/laser1.wav",sfxHdl));
}

void EnemyBulletManager::Render(){
	PoolAllocator::Render();
}

void EnemyBulletManager::reset() {
	PoolAllocator::reset();
}

void EnemyBulletManager::spawnBullet(float x, float y, float minSpeed, int numBullets /*= 1*/){
	for(int i = 0; i<totalObjects_; i++){
		if(!objects_[i]->GetActive()){
			objects_[i]->spawn(x, y, minSpeed); Sound::Get()->PlayWaveFile(sfxHdl, false, 0.5f); break;
		}
	}
}

void EnemyBulletManager::Update( float elapsedSec )
{
	PoolAllocator::Update(elapsedSec);
}

EnemyBulletManager::~EnemyBulletManager()
{
	Release();
}

void EnemyBulletManager::Release()
{
	meshEnemyBullet_.Release();
}

void EnemyBulletManager::PostRender()
{
	PoolAllocator::PostRender();
}
