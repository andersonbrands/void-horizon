#include "SpaceDust.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "Utils.h"
#include "GameObject.h"
#include "Player.h"
#include "Game.h"

using namespace MSH;
using namespace D3D;
using namespace VEC;
using namespace std;


void SpaceDust::Initialise()
{
	meshStar_.InitialiseMesh(GetD3DManager()->GetD3DDevice(), "bin/data/", "star.x");

	for(int i=0;i<totalObjects_;++i){
		objects_[i]->Initialise(meshStar_);
	}

}

void SpaceDust::Render()
{
	PoolAllocator::Render();
}

void SpaceDust::Update( float elapsedSec )
{
	PoolAllocator::Update(elapsedSec);
}

SpaceDust::~SpaceDust()
{
	Release();
}

void SpaceDust::Release()
{
	meshStar_.Release();
}

void SpaceDust::PostRender()
{
	PoolAllocator::PostRender();
}

