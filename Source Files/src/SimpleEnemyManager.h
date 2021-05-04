#ifndef SIMPLE_ENEMY_MANAGER_H
#define SIMPLE_ENEMY_MANAGER_H

#include "Mesh.h"
#include "Vector2.h"
#include "GameObject.h"
#include "SimpleEnemy.h"
#include "RandomNumberGenerator.h"
#include "Player.h"
#include "ExplosionManager.h"
#include "EnemyBulletManager.h"

#include "PoolAllocator.h"



class SimpleEnemyManager : public PoolAllocator<SimpleEnemy> {
    public:
		SimpleEnemyManager(Player& p, ExplosionManager& em): PoolAllocator<SimpleEnemy>(15), MAX_ENEMIES_PER_WAVE(7), player_(p), explosionManager_(em), enemyBulletManager_(p, em) {};
        ~SimpleEnemyManager();

        //load enemy models, create instances
        virtual void Initialise();
        //tidy up on way out
        void Release();
        //move enemies and let them update
        void Update(float elapsedSec);
        //show all enemies
        void Render();
        //sprites, additive effects, etc
        void PostRender();
        void reset();

		EnemyBulletManager &GetEnemyBulletManager() {
			return enemyBulletManager_;
		}

    private:
		//PoolAllocator<SimpleEnemy> poolAllocator_;

        //instances share the same mesh
        MSH::Mesh meshSimpleEnemy_;

		const int MAX_ENEMIES_PER_WAVE;

        static RandomNumberGenerator rng_;

        EnemyBulletManager enemyBulletManager_;

        void sendWave(int numEnemies);
        Player& player_;
        ExplosionManager& explosionManager_;
};


#endif