#include "Star.h"

#include <map>

#include "D3DFuncs.h"
#include "D3DManager.h"
#include "Mesh.h"
#include "Sound.h"
#include "Error.h"
#include "Player.h"
#include "Game.h"

using namespace MSH;
using namespace VEC;
using namespace D3D;
using namespace std;
using namespace UDEF;



RandomNumberGenerator Star::rng_;

Star::Star() : GameObject(GameTypes::STAR), SPEED(0.35f)/*, scaleX(0.05f),scaleY(0.04f)*/, scale(0.05f), rotation(0.0f), rotationGain(0.01f)
{
}

Star::~Star()
{
	
}


void Star::Update( float elapsedSec )
{
	mPos.x -= SPEED;
	if(mPos.x<BORDER_LEFT)
		spawn();


//#define ENABLE_STAR_EFFECT
#ifdef ENABLE_STAR_EFFECT
	rotation += rotationGain;
	if (rotation > g_2PI){
		rotation = 0.0f;
		rotationGain += 0.04f;
		if(rotationGain > g_PI){
			rotationGain = 0.01f;
		}
	}
#undef ENABLE_STAR_EFFECT
#endif // ENABLE_STAR_EFFECT

}

void Star::spawn(bool randomizeX /*= false*/){
	if (randomizeX) {
		mPos.x = rng_.getRandomValue(BORDER_RIGHT + 40) - 35;
	} else {
		mPos.x = BORDER_RIGHT;
	}

	int randomValue(rng_.getRandomValue(30));
	float percentage( (randomValue%2) ? randomValue/100.0f:-randomValue/100.0f );
	scale = 0.025f * (1 + percentage);

	mPos.y = rng_.getRandomValue(BORDER_TOP);
}


void Star::Render()
{
	vector<D3DMATERIAL9> mtls;
	D3DMATERIAL9 mtl;
	CreateMaterial(1,1,1,mtl);
	mtls.insert(mtls.begin(), mpMesh->GetNumMaterials(),mtl);

	D3DXMATRIX TranslateMat, ScaleMat, WorldMat, RotateMat;
	D3DXMatrixRotationX(&RotateMat, UDEF::D2R(180));
	RenderMesh(D3DManager::Get()->GetD3DDevice(), *mpMesh, &mtls[0], nullptr, mPos.x,mPos.y,1.0f, scale,scale,scale, 0, 0, rotation);
}

void Star::Initialise(MSH::Mesh &myMesh)
{
	mpMesh=&myMesh;
	spawn(true);
}

void Star::PostRender()
{
}


