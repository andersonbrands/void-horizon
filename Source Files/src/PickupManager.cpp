#include "PickupManager.h"
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


RandomNumberGenerator PickupManager::rng_;

void PickupManager::Initialise(){
	meshPickup_.InitialiseMesh(GetD3DManager()->GetD3DDevice(), "bin/data/", "health.x");

	for(int i=0;i<totalObjects_;++i){
		objects_[i]->Initialise(meshPickup_);
	}
}

void PickupManager::Render(){
	PoolAllocator::Render();
}

void PickupManager::sendWave(int numPickups){
	if (numPickups > MAX_PICKUPS_PER_WAVE)
		numPickups = MAX_PICKUPS_PER_WAVE;

	float offset = BORDER_TOP/numPickups/2;
	int availablePickupIndex(0);
	for (int i(availablePickupIndex); ( i < numPickups && availablePickupIndex < (int)objects_.size() );){
		if (!objects_[availablePickupIndex]->isAlive()){
			objects_[availablePickupIndex]->spawn(/*BORDER_RIGHT + 5, BORDER_TOP/numPickups * i + offset*/);
			++i;
		}
		++availablePickupIndex;
	}
}


void PickupManager::Update( float elapsedSec )
{
	PoolAllocator::Update(elapsedSec);

	int activePickupsCount = 0;

	for(int i=0;i<(int)objects_.size();++i){
		if(objects_[i]->isAlive()){
			++activePickupsCount;
		}
	}

	if(activePickupsCount == 0 && Game::Get()->GetPlayer().hp_<100){

		int chance = rng_.getRandomValue(500);
		if (chance <= 3)
			sendWave(1);
	}
}

PickupManager::~PickupManager()
{
	Release();
}

void PickupManager::Release()
{
	meshPickup_.Release();
}

void PickupManager::reset() {
	PoolAllocator::reset();
}

void PickupManager::PostRender()
{
	PoolAllocator::PostRender();
}

