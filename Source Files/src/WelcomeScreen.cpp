#include "WelcomeScreen.h"

WelcomeScreen::WelcomeScreen() {
    title_ = new Sprite;
}

WelcomeScreen::~WelcomeScreen() {
    delete title_;
}


void WelcomeScreen::Update( float elapsedSec ) {

}

void WelcomeScreen::Render() {
	title_->Render();
}

void WelcomeScreen::Initialise() {
    title_->mPos = Vec2(30, 65);
    title_->mWorldSize = Vec2(110.5f,8.7f);
	title_->mFlags = RenderFlags::ALPHA_BLEND_ENABLE;//|RenderFlags::NO_Z_BUFFER;
	title_->Load("bin/data/void_horizon.png");
}

void WelcomeScreen::PostRender() {
}


