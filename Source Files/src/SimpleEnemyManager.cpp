#include "SimpleEnemyManager.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "Utils.h"
#include "GameObject.h"
#include "Player.h"
#include "Game.h"
#include "defines.h"

using namespace MSH;
using namespace D3D;
using namespace VEC;
using namespace std;
using namespace UDEF;


RandomNumberGenerator SimpleEnemyManager::rng_;

void SimpleEnemyManager::Initialise(){
	meshSimpleEnemy_.InitialiseMesh(GetD3DManager()->GetD3DDevice(), "bin/data/", "enemy.x");


	for(int i=0;i<totalObjects_;++i){
		objects_[i]->Initialise(meshSimpleEnemy_);
	}

	enemyBulletManager_.Initialise();
}

void SimpleEnemyManager::Render(){
	PoolAllocator::Render();
	enemyBulletManager_.Render();
}

void SimpleEnemyManager::sendWave(int numEnemies){
	if (numEnemies > MAX_ENEMIES_PER_WAVE)
		numEnemies = MAX_ENEMIES_PER_WAVE;

	float offset = BORDER_TOP/numEnemies/2;
	int availableEnemyIndex(0);
	for (int i(availableEnemyIndex); ( i < numEnemies && availableEnemyIndex < (int)objects_.size() );){
		if (!objects_[availableEnemyIndex]->isAlive()){
			objects_[availableEnemyIndex]->spawn(BORDER_RIGHT + 5, BORDER_TOP/numEnemies * i + offset);
			++i;
		}
		++availableEnemyIndex;
	}
}


void SimpleEnemyManager::Update( float elapsedSec )
{
	PoolAllocator::Update(elapsedSec);

	int activeEnemiesCount = 0;

	for(int i=0;i<(int)objects_.size();++i){
		if(objects_[i]->isAlive()){
			++activeEnemiesCount;
		}
	}

	if(activeEnemiesCount == 0){

		int chance = rng_.getRandomValue(100);
		if (chance <= 5)
			sendWave(rng_.getRandomValue(MAX_ENEMIES_PER_WAVE));
	}

	enemyBulletManager_.Update(elapsedSec);
}

SimpleEnemyManager::~SimpleEnemyManager()
{
	Release();
}

void SimpleEnemyManager::Release()
{
	meshSimpleEnemy_.Release();
}

void SimpleEnemyManager::reset(){
	PoolAllocator::reset();
	enemyBulletManager_.reset();
}

void SimpleEnemyManager::PostRender()
{
	PoolAllocator::PostRender();
	enemyBulletManager_.PostRender();
}

