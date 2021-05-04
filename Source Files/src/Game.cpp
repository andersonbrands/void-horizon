#include <d3dx9.h>		
#include <map>

#include "Game.h"
#include "D3DManager.h"
#include "DInputManager.h"
#include "D3DFuncs.h"
#include "Error.h"
#include "Player.h"
#include "SpaceDust.h"
#include "Sound.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Utils.h"
#include "SimpleEnemyManager.h"
#include "PickupManager.h"

using namespace D3D; 
using namespace VEC;
using namespace std;
using namespace MSH;


Game *Game::spGame=nullptr;

/* ---------------------------------------------------------------------------------
Going from windowed->fullscreen we lose control of the hardware temporarily
so we need to release certain unmanaged resources (like fonts) just before it
happens, then allocate them again just after the hardware is back. In this case
there is nothing to do, so the functions we supply to D3D don't do anything.
*/

void OnResetDevice(void *pMe)
{
	MPOD_ASSERT(pMe);
	Game *p=static_cast<Game*>(pMe);
	p->OnResetDevice();
}

void OnLostDevice(void *pMe)
{
	MPOD_ASSERT(pMe);
	Game *p=static_cast<Game*>(pMe);
	p->OnLostDevice();
}

void Game::OnLostDevice()
{
	hud_->OnLostDevice();
}

void Game::OnResetDevice()
{
	hud_->OnResetDevice();
}


void Game::Initialise()
{
	SeedRandom();
	//some resources need to be released and re-acquired when we go window->full screen
	//or full screen->window
	//mainly fonts. So we can use these two functions to drop and re-initialise anything
	D3DManager::Get()->SetResetAndLost(::OnLostDevice, ::OnResetDevice, this);

	welcomeScreen_.Initialise();
	gameoverScreen_.Initialise();

	// create space dust
	pSpaceDust_ = new SpaceDust;
	pSpaceDust_->Initialise();

	//create one player
	mpPlayer = new Player;
	mpPlayer->Initialise();

	hud_ = new InGameHUD(*mpPlayer);
	hud_->Initialise();

	explosionManager_ = new ExplosionManager;
	explosionManager_->Initialise();

	sem_ = new SimpleEnemyManager((*mpPlayer), (*explosionManager_));
	sem_->Initialise();

	pm_ = new PickupManager();
	pm_->Initialise();

	background_ = new Sprite;

	background_->mPos = Vec2(30,40);
	background_->mZ = 300;
    background_->mWorldSize = Vec2(GetD3DManager()->GetBufferWidth(),GetD3DManager()->GetBufferHeight());
	background_->mFlags = RenderFlags::ALPHA_BLEND_ENABLE;//|RenderFlags::NO_Z_BUFFER;
	background_->Load("bin/data/background.png");

	pausedSp_ = new Sprite;

	pausedSp_->mPos = Vec2(30,40);
	pausedSp_->mZ = 10;
    pausedSp_->mWorldSize = Vec2(90.2f,10.3f);
	pausedSp_->mFlags = RenderFlags::ALPHA_BLEND_ENABLE|RenderFlags::NO_Z_BUFFER;
	pausedSp_->Load("bin/data/paused.png");

	//music
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/Dark.wav",welcomeScreenMusic_));
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/Techno.wav",inGameMusic_));
	MPOD_VERIFY(Sound::Get()->LoadWaveFile("bin/data/audio/GG.wav",gameOverMusic_));

	Sound::Get()->PlayWaveFile(welcomeScreenMusic_, true, 0.8f);

	GetD3DManager()->SetBackColour(0,0,0);
}



void Game::ProcessInput(float elapsedSec)
{
	extern bool g_okToQuit;
	//if(DInputManager::Get()->Pressed(DIK_ESCAPE))
		//g_okToQuit=true;

	
	//for debugging allow the camera and camera target to be moved
	//press F1 to switch between moving the camera and the target
	static bool sCam=true;
	if(DInputManager::Get()->Pressed(DIK_F1))
		sCam=!sCam;

	float sp = 10 * elapsedSec;
	D3DXVECTOR3 inc(0,0,0);
	if(DInputManager::Get()->KeyDown(DIK_W))
		inc.z+=sp;
	if(DInputManager::Get()->KeyDown(DIK_S))
		inc.z-=sp;
	if(DInputManager::Get()->KeyDown(DIK_A))
		inc.x-=sp;
	if(DInputManager::Get()->KeyDown(DIK_D))
		inc.x+=sp;
	if(DInputManager::Get()->KeyDown(DIK_E))
		inc.y+=sp;
	if(DInputManager::Get()->KeyDown(DIK_C))
		inc.y-=sp;

	if(sCam)
		mCamPos+=inc;
	else
		mCamTgt+=inc;
	
	switch (currentMode_) {
	case Game::WELCOME_SCREEN:
		if(DInputManager::Get()->KeyDown(DIK_SPACE)){
			reset();
			currentMode_ = IN_GAME;
			Sound::Get()->StopWaveFile(welcomeScreenMusic_);
			Sound::Get()->PlayWaveFile(inGameMusic_, true, 0.8f);
			break;
		}
		if(DInputManager::Get()->Pressed(DIK_ESCAPE)){
			g_okToQuit=true;
			break;
		}
		break;
	case Game::IN_GAME:
		if(DInputManager::Get()->Pressed(DIK_ESCAPE)){
			reset();
			currentMode_ = WELCOME_SCREEN;
			Sound::Get()->StopWaveFile(inGameMusic_);
			Sound::Get()->PlayWaveFile(welcomeScreenMusic_, true, 0.8f);
			break;
		}
		if(DInputManager::Get()->Pressed(DIK_P)){
			paused_ = !paused_;
		}
		break;
	case Game::GAME_OVER:
		if(DInputManager::Get()->KeyDown(DIK_SPACE)){
			reset();
			currentMode_ = IN_GAME;
			Sound::Get()->StopWaveFile(gameOverMusic_);
			Sound::Get()->PlayWaveFile(inGameMusic_, true, 0.8f);
			break;
		}
		if(DInputManager::Get()->Pressed(DIK_ESCAPE)){
			currentMode_ = WELCOME_SCREEN;
			Sound::Get()->StopWaveFile(gameOverMusic_);
			Sound::Get()->PlayWaveFile(welcomeScreenMusic_, true, 0.8f);
			break;
		}
		break;
	default:
		break;
	}
}


void Game::Update(float elapsedSec)
{
	mClock+=elapsedSec;
	ProcessInput(elapsedSec);

	switch (currentMode_)
	{
	case Game::WELCOME_SCREEN:
		welcomeScreen_.Update(elapsedSec);
		//
		break;
	case Game::IN_GAME:
		if (!paused_){
			pSpaceDust_->Update(elapsedSec);

			mpPlayer->Update(elapsedSec);
	
			sem_->Update(elapsedSec);

			pm_->Update(elapsedSec);

			explosionManager_->Update(elapsedSec);
		}
		break;
	case Game::GAME_OVER:
		gameoverScreen_.Update(elapsedSec);
		//
		break;
	default:
		break;
	}
	
}

void Game::reset(){
	paused_ = false;
	mpPlayer->reset();
	sem_->reset();
	pm_->reset();

	explosionManager_->reset();
}


void Game::gameOver(){
	currentMode_ = GAME_OVER;
	Sound::Get()->StopWaveFile(inGameMusic_);
	Sound::Get()->PlayWaveFile(gameOverMusic_, false, 0.8f);
}


void Game::Render(float elapsedSec)
{
	D3DManager::Get()->BeginScene();
	LPDIRECT3DDEVICE9 d3d = D3DManager::Get()->GetD3DDevice();
	SetupViewMatrices(mCamPos.x,mCamPos.y,mCamPos.z, mCamTgt.x,mCamTgt.y,mCamTgt.z);

	//turn on texture filtering so things look a bit nicer
	d3d->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	d3d->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3d->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//over the top lighting makes things glow
	d3d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
	//ensure transparency is off
	d3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//ensure z buffer is on
	d3d->SetRenderState(D3DRS_ZENABLE, TRUE);
	//if meshes are changing size we need to fix the normals
	d3d->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	float lightValue(1.0f);
	//main scene light
	SetupDirectionalLight(0,true, lightValue,lightValue,lightValue, D3DXVECTOR3(0,0,30), 100, 0.01f);
	//ambient light
	//float amb=0.2f;
	//SetupAmbient(amb,amb,amb);

	background_->Render();
	
	
	switch (currentMode_)
	{
	case Game::WELCOME_SCREEN:
		welcomeScreen_.Render();
		hud_->RenderWelcomeScreenText();
		break;
	case Game::IN_GAME:
		pSpaceDust_->Render();
		mpPlayer->Render();
		sem_->Render();
		pm_->Render();

		explosionManager_->Render();

		hud_->Render();

		if (paused_) 
			pausedSp_->Render();

		welcomeScreen_.PostRender();
		gameoverScreen_.PostRender();

		pSpaceDust_->PostRender();
		mpPlayer->PostRender();
		sem_->PostRender();
		pm_->PostRender();
		explosionManager_->PostRender();
		break;
	case Game::GAME_OVER:
		gameoverScreen_.Render();
		hud_->RenderGameOverScreenText();
		break;
	default:
		break;
	}

	D3DManager::Get()->EndScene();
}


void Game::Cleanup()
{

	MPOD_ASSERT(mpPlayer);
	delete mpPlayer;
	mpPlayer=nullptr;

	
	MPOD_ASSERT(hud_);
	delete hud_;
	hud_=nullptr;
	

	MPOD_ASSERT(pSpaceDust_);
	delete pSpaceDust_;
	pSpaceDust_=nullptr;

	MPOD_ASSERT(sem_);
	delete sem_;
	sem_=nullptr;

	MPOD_ASSERT(pm_);
	delete pm_;
	pm_=nullptr;

	MPOD_ASSERT(explosionManager_);
	delete explosionManager_;
	explosionManager_=nullptr;

	delete background_;
	delete pausedSp_;
}

Game::Game() : mClock(0), mCamPos(30,35,-100), mCamTgt(30,35,0), currentMode_(WELCOME_SCREEN), welcomeScreen_(), gameoverScreen_(), paused_(false)
{
	MPOD_ASSERT(!spGame);
	spGame=this;
	Initialise();
}

Game::~Game()
{
	spGame=nullptr;
	Cleanup();
}