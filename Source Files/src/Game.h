#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>

#include "Error.h"

class Player;
class Enemy;
class SpaceDust;
class PickupManager;

#include "InGameHUD.h"
#include "ExplosionManager.h"
#include "WelcomeScreen.h"
#include "GameOverScreen.h"
#include "SimpleEnemyManager.h"
#include "EnemyBulletManager.h"

namespace GameTypes { typedef enum { 
	PLAYER, 
	FIGHTER,
	STAR,
	PICKUP,
	EXPLOSION
} Enum; }

class Game
{
public:

	Game();
	~Game();
	//logic and movement
	void Update(float elapsedSec);
	//show everything
	void Render(float elapsedSec);
	
	//accessors
	Player &GetPlayer() {
		MPOD_ASSERT(mpPlayer);
		return *mpPlayer;
	}

	ExplosionManager &GetExplosionManager(){
		MPOD_ASSERT(explosionManager_);
		return *explosionManager_;
	}

	EnemyBulletManager &GetEnemyBulletManager() {
		return sem_->GetEnemyBulletManager();
	}

	float GetClock() const {
		return mClock;
	}
	int GetLevel() const {
		return mLevel;
	}

	//singleton pattern (get hold of the game)
	static Game *Get() {
		MPOD_ASSERT(spGame);
		return spGame;
	}

	void gameOver();
	void reset();

	//go full screen or windowed and some resource may need reloading
	void OnResetDevice();
	void OnLostDevice();

private:
	enum GameMode{
		WELCOME_SCREEN,
		IN_GAME,
		GAME_OVER
	};

	int welcomeScreenMusic_, inGameMusic_, gameOverMusic_;

	Sprite *background_;
	Sprite *pausedSp_;

	GameMode currentMode_;

	bool paused_;

	WelcomeScreen welcomeScreen_;
	GameOverScreen gameoverScreen_;

	//singleton
	static Game *spGame;

	// Space dust
	SpaceDust *pSpaceDust_;

	//SimpleEnemyManager
	SimpleEnemyManager *sem_;

	//PickupManager
	PickupManager *pm_;

	// ExplosionManager
	ExplosionManager *explosionManager_;

	InGameHUD *hud_;

	//one player
	Player *mpPlayer;

	//count up in milli-sec
	float mClock;
	//what level are we on
	int mLevel;
	//where is the camera
	D3DXVECTOR3 mCamPos, mCamTgt;

	//handle debug input, escape, etc
	void ProcessInput(float elapsedSec);
	//tidy up on way out
	void Cleanup();
	//load things in and get ready
	void Initialise();

};

#endif
