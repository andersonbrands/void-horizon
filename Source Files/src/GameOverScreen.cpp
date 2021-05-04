#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() {
    title_ = new Sprite;
	gameOver_ = new Sprite;
}

GameOverScreen::~GameOverScreen() {
    delete title_;
	delete gameOver_;
}


void GameOverScreen::Update( float elapsedSec ) {

}

void GameOverScreen::Render() {
    title_->Render();
	gameOver_->Render();
}

void GameOverScreen::Initialise() {
    title_->mPos = Vec2(30, 65);
    title_->mWorldSize = Vec2(110.5f,8.7f);
    title_->mFlags = RenderFlags::ALPHA_BLEND_ENABLE;//|RenderFlags::NO_Z_BUFFER;
    title_->Load("bin/data/void_horizon.png");


	gameOver_->mPos = Vec2(30, 45);
    gameOver_->mWorldSize = Vec2(110.5f,8.7f);
    gameOver_->mFlags = RenderFlags::ALPHA_BLEND_ENABLE;//|RenderFlags::NO_Z_BUFFER;
    gameOver_->Load("bin/data/game_over.png");
}

void GameOverScreen::PostRender() {
}


