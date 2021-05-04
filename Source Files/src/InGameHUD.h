#ifndef IN_GAME_HUD_H
#define IN_GAME_HUD_H

#include "Vector2.h"
#include "D3DManager.h"
#include "Player.h"
#include "Font.h"
#include <sstream>

using namespace D3D;
using namespace VEC;


class InGameHUD {
    public:
        //constructors
        InGameHUD(Player&);
        ~InGameHUD();

		void Initialise();
        void Render() const;

		void RenderWelcomeScreenText() const;
		void RenderGameOverScreenText() const;

        //if we switch from window to full screen we can lose certain
        //resources, use these functions to release and recreate them (fonts)
        void OnResetDevice();
        void OnLostDevice();

    private:
        const Player& player_;
        void drawHealthBar() const;
		RECT drawFrame(std::string label, int x, int y, int height) const;
        void fillHealthBar() const;
        void drawRect(float, float, float, float, D3DCOLOR) const;
        void drawLine(Vec2, Vec2, float, D3DCOLOR) const;

        Font gFont;
};
#endif