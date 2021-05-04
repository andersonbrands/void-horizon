#include <d3dx9.h>	
#include <vector>

#include "Bullet.h"
#include "Player.h"
#include "DInputManager.h"
#include "D3DManager.h"
#include "D3DFuncs.h"
#include "defines.h"
#include "GameObject.h"
#include "EFighter.h"
#include "Sound.h"
#include "Game.h"

using namespace std;
using namespace D3D;
using namespace MSH;
using namespace VEC;

/*Bullet::Bullet() : GameObject("BULLET")
{
	SetVelocityLimit(1000);
	mpSprite = new Sprite();
	mpSprite->Load("data/blue_shot.dds");
}*/