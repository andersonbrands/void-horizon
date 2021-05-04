#include "PlayerBulletManager.h"
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


RandomNumberGenerator PlayerBulletManager::rng_;

void PlayerBulletManager::Initialise(){
	meshPlayerBullet_.InitialiseMesh(GetD3DManager()->GetD3DDevice(), "bin/data/", "bullet.x");

	for(int i=0;i<totalObjects_;++i){
		objects_[i]->Initialise(meshPlayerBullet_);
	}

	//sound effect
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/laser2.wav",sfxHdl));

}

void PlayerBulletManager::Render(){
	PoolAllocator::Render();
}

void PlayerBulletManager::reset() {
	PoolAllocator::reset();
	//for(int i = 0; i<TOTAL_BULLETS; i++){
	//	playerBullets_[i]->remove();
	//}
}

void PlayerBulletManager::spawnBullet(float x, float y, int numBullets /*= 1*/){
	for(int i = 0; i<totalObjects_; i++){
		if(!objects_[i]->GetActive()){
			objects_[i]->spawn(x, y); Sound::Get()->PlayWaveFile(sfxHdl, false, 0.5f); break;
		}
	}
}

void PlayerBulletManager::Update( float elapsedSec )
{
	PoolAllocator::Update(elapsedSec);
}

PlayerBulletManager::~PlayerBulletManager()
{
	Release();
}

void PlayerBulletManager::Release()
{
	meshPlayerBullet_.Release();
}

void PlayerBulletManager::PostRender()
{
	PoolAllocator::PostRender();
}
