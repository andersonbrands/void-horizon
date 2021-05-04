#include <map>

#include "PlayerBullet.h"
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


PlayerBullet::~PlayerBullet()
{
}

PlayerBullet::PlayerBullet() : mpMesh(nullptr), GameObject(GameTypes::FIGHTER), PLAYER_BULLET_SPEED(2.0f)
{
}

void PlayerBullet::Update( float elapsedSec )
{
	if (GetActive()){
	mPos.x += PLAYER_BULLET_SPEED;
	if(mPos.x>BORDER_RIGHT)
		remove();
	}
}

void PlayerBullet::Initialise( MSH::Mesh &myMesh )
{
	mpMesh=&myMesh;
}

void PlayerBullet::reset() {
	remove();
}

void PlayerBullet::PostRender()
{
}

void PlayerBullet::Render()
{
	if (GetActive()){
		vector<D3DMATERIAL9> mtls;
		D3DMATERIAL9 mtl;
		CreateMaterial(1,1,1,mtl);
		mtls.insert(mtls.begin(),mpMesh->GetNumMaterials(),mtl);

		float scale=0.15f;
		RenderMesh(GetD3DManager()->GetD3DDevice(), *mpMesh, &mtls[0], nullptr, mPos.x, mPos.y, 0, scale,scale,scale, UDEF::g_PI/1,0,UDEF::g_PI/1);
	}
}

void PlayerBullet::spawn(float x, float y){
	SetActive (true);

	mPos.x = x;
	mPos.y = y + 1;
}

void PlayerBullet::remove(){
	SetActive (false);
}