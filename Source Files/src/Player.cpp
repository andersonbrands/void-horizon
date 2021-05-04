#include <d3dx9.h>	
#include <vector>

#include "Player.h"
#include "DInputManager.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "defines.h"
#include "GameObject.h"
#include "Sound.h"
#include "Game.h"

using namespace std;
using namespace D3D;
using namespace MSH;
using namespace VEC;
using namespace UDEF;

Player::Player() : GameObject(GameTypes::PLAYER), playerBulletManager_(), maxSpeed_(1.0f), 
					speed_(), rotation_(g_PI/2, g_PI), SPEED_STEP(0.2f), FRICTION(0.93f), hp_(100),
					score_(0), destroyedEnemiesCount_(0), missedEnemiesCount_(0)
{
	spawn(10.f, 35.0f);
}

Player::~Player()
{
	Release();
}

void Player::Update( float elapsedSec )
{

	if(DInputManager::Get()->KeyDown(DIK_DOWNARROW))
		speed_.y-= SPEED_STEP;
	else if(DInputManager::Get()->KeyDown(DIK_UPARROW))
		speed_.y+=SPEED_STEP;
	else
		speed_.y *= FRICTION;

	if(DInputManager::Get()->KeyDown(DIK_LEFTARROW))
		speed_.x-=SPEED_STEP;
	else if(DInputManager::Get()->KeyDown(DIK_RIGHTARROW))
		speed_.x+=SPEED_STEP;
	else
		speed_.x *= FRICTION;

	mPos += speed_;
	
	rotation_.x = g_PI/2 + speed_.x/5;

	rotation_.y = g_PI - speed_.y/3;

	// restricting speed X axis
	if (speed_.x > maxSpeed_)
		speed_.x = maxSpeed_;
	else if (speed_.x < -maxSpeed_)
		speed_.x = -maxSpeed_;
	// restricting speed Y axis
	if (speed_.y > maxSpeed_)
		speed_.y = maxSpeed_;
	else if (speed_.y < -maxSpeed_)
		speed_.y = -maxSpeed_;


	if(mPos.y<BORDER_DOWN)
		mPos.y=BORDER_DOWN;
	else if(mPos.y>BORDER_TOP)
		mPos.y=BORDER_TOP;
	
	if(mPos.x<BORDER_LEFT)
		mPos.x=BORDER_LEFT;
	else if(mPos.x>BORDER_RIGHT)
		mPos.x=BORDER_RIGHT;

	if(DInputManager::Get()->Pressed(DIK_Z)){
		playerBulletManager_.spawnBullet(mPos.x,mPos.y);
	}
	playerBulletManager_.Update(elapsedSec);
}


void Player::Render()
{
	vector<D3DMATERIAL9> mtls;
	D3DMATERIAL9 mtl;
	CreateMaterial(1,1,1,mtl);
	mtls.insert(mtls.begin(),mMesh.GetNumMaterials(),mtl);

	float scale=0.2f;
	D3DXMATRIX TranslateMat, ScaleMat, WorldMat, RotateMat;
	D3DXMatrixRotationX(&RotateMat, UDEF::D2R(0));
	D3DXMatrixRotationY(&RotateMat, UDEF::D2R(0));
	
	RenderMesh(D3DManager::Get()->GetD3DDevice(), mMesh, &mtls[0], nullptr, mPos.x,mPos.y,0, scale,scale,scale,rotation_.x, rotation_.y, UDEF::g_PI/2);
	playerBulletManager_.Render();
}

void Player::Initialise()
{
	mMesh.InitialiseMesh(D3DManager::Get()->GetD3DDevice(), "bin/data/", "player.x");
	playerBulletManager_.Initialise();
}

void Player::damage(int damage){
	hp_ -= damage;
	if (hp_ <= 0){
		kill();
		hp_ = damage;
	}
}

void Player::heal(){
	if (hp_ < 100){
		hp_ += 20;
		if (hp_ > 100) {
			hp_ = 100;
		}
	}
}

void Player::kill(){
	Game::Get()->gameOver();
}

void Player::reset(){
	playerBulletManager_.reset();
	maxSpeed_ = 1.0f; 
	hp_ = 100;
	score_ = 0;
	destroyedEnemiesCount_ = 0;
	missedEnemiesCount_ = 0;

	mPos.x = 10.f;
	mPos.y = 35.0f;
}

void Player::spawn(float x, float y){
	SetActive (true);

	mPos.x = x;
	mPos.y = y;
}

void Player::destroyEnemy(){
	destroyedEnemiesCount_++;
	score_ += 25;
}

void Player::missEnemy(){
	missedEnemiesCount_++;
	score_ -= 5;
}

void Player::getPickup(){
	//destroyedEnemiesCount_++;
}

void Player::missPickup(){
	//missedEnemiesCount_++;
}

void Player::Release()
{
	mMesh.Release();
	playerBulletManager_.Release();
}

void Player::PostRender()
{
	playerBulletManager_.PostRender();
}


