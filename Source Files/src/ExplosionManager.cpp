#include "ExplosionManager.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "Utils.h"
#include "Sound.h"
#include "Game.h"
#include "defines.h"

using namespace MSH;
using namespace D3D;
using namespace VEC;
using namespace std;
using namespace UDEF;


RandomNumberGenerator ExplosionManager::rng_;

void ExplosionManager::Initialise() {
	
}

void ExplosionManager::Render() {
	PoolAllocator::Render();
}

void ExplosionManager::reset() {
	PoolAllocator::reset();
}

void ExplosionManager::explode(float x, float y) {
	for(int i = 0; i<totalObjects_; i++) {
        if(!objects_[i]->GetActive()) {
			objects_[i]->Play(Vec2(x, y),false);
            break;
        }
    }
}

void ExplosionManager::Update( float elapsedSec ) {
	PoolAllocator::Update(elapsedSec);
}

ExplosionManager::~ExplosionManager() {
    Release();
}

void ExplosionManager::Release() {

}

void ExplosionManager::PostRender() {
	PoolAllocator::PostRender();
}
