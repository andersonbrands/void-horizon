#include "InGameHUD.h"

InGameHUD::InGameHUD(Player& player) : player_(player) {

}


InGameHUD::~InGameHUD() {
    gFont.Release();
}

//fonts get lost when reseting due to window/full screen switch
void InGameHUD::OnLostDevice() {
    gFont.OnLostDevice();
}

void InGameHUD::OnResetDevice() {
    gFont.OnResetDevice();
}

void InGameHUD::Initialise() {
    //initialise the font we'll use, needs replacing with an artist created one
    gFont.Init(GetD3DManager()->GetD3DDevice(), "Lucida Console", 16, true);
}

void InGameHUD::RenderWelcomeScreenText() const {
	int w(GetD3DManager()->GetBufferWidth());
    //int h(GetD3DManager()->GetBufferHeight());

    std::stringstream  ss;
	
	int x( (w-418) / 2), y(330), step(30);

	ss << "Anderson Brandao | Ashly Brown | Linyan Liu | Adam Rusling";
	gFont.Print(ss.str().c_str(),x - 105, 250, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 700, 16);

	ss.str("");
	ss << "2013-2014";
	gFont.Print(ss.str().c_str(),x + 159, 275, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 110, 16);

	ss.str("");
	ss << "Move - Left, Up, Right and Down arrows";
	gFont.Print(ss.str().c_str(),x, y+20, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 418, 16);
	y += 2 * step;

	ss.str("");
	ss << "Shoot - Z";
	gFont.Print(ss.str().c_str(),x + 159, y, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 100, 16);
	y += step;

	ss.str("");
	ss << "Pause - P";
	gFont.Print(ss.str().c_str(),x + 159, y, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 100, 16);
	y += 3 * step;

	ss.str("");
	ss << "Quit - ESC";
	gFont.Print(ss.str().c_str(),x + 154, y, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 110, 16);

	ss.str("");
	ss << "Press SPACE to Play";
	gFont.Print(ss.str().c_str(), ((w-210)/2), 600, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 210, 16);

}

void InGameHUD::RenderGameOverScreenText() const {
	int w(GetD3DManager()->GetBufferWidth());
    int h(GetD3DManager()->GetBufferHeight());

    std::stringstream  ss;
	
	int wOffset(450);

	RECT frame(drawFrame("Final Score", w * 0.06f + wOffset, h * 0.55f, 40));
	ss.str("");
	ss << player_.score_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/2, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 60, 16);

	frame = drawFrame("Enemies", w * 0.08f + wOffset, h * 0.65f, 60);
	ss.str("");
	ss << "missed: "<< player_.missedEnemiesCount_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/5, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 180, 16);
	ss.str("");
	ss << "destroyed: "<< player_.destroyedEnemiesCount_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/1.5f, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 180, 16);

	ss.str("");
	ss << "Initial screen - ESC";
	gFont.Print(ss.str().c_str(),((w-220)/2), 580, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 220, 16);

	ss.str("");
	ss << "Press SPACE to Play again";
	gFont.Print(ss.str().c_str(), ((w-276)/2), 650, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 276, 16);
}

void InGameHUD::Render() const {
    int w(GetD3DManager()->GetBufferWidth());
    int h(GetD3DManager()->GetBufferHeight());

    drawHealthBar();
    fillHealthBar();

    std::stringstream  ss;

	RECT frame(drawFrame("Score", w * 0.02f, h * 0.055f, 40));
	ss.str("");
	ss << player_.score_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/2, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 60, 16);

	frame = drawFrame("Enemies", w * 0.02f, h * 0.85f, 60);
	ss.str("");
	ss << "missed: "<< player_.missedEnemiesCount_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/5, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 180, 16);
	ss.str("");
	ss << "destroyed: "<< player_.destroyedEnemiesCount_;
	gFont.Print(ss.str().c_str(),frame.left + 10, frame.top + (frame.bottom - frame.top)/1.5f, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, 180, 16);
}

RECT InGameHUD::drawFrame(std::string label, int x, int y, int height) const {
	std::stringstream  ss;

	int labelWidth(label.size() * 12);

	DWORD frameColor(D3DCOLOR_XRGB( 255, 255, 255 ));

	ss << label;

	RECT rect = gFont.Print(ss.str().c_str(),x, y, D3DCOLOR_XRGB( 255, 255, 255 ), NULL, labelWidth, 16);

	int frameL(rect.left - 12);
	int frameT(rect.top + (rect.bottom - rect.top) / 2);
	int frameR(rect.right + 100);
	int frameB(frameT + height);

	RECT frame = {frameL,frameT,frameR,frameB};

	drawLine(Vec2(frameL, frameT),Vec2(frameL + 10, frameT),1, frameColor);
	drawLine(Vec2(frameL, frameT),Vec2(frameL, frameB),1, frameColor);
	drawLine(Vec2(frameL, frameB),Vec2(frameR, frameB),1, frameColor);
	drawLine(Vec2(frameR, frameB),Vec2(frameR, frameT),1, frameColor);
	drawLine(Vec2(frameR, frameT),Vec2(rect.right + 2, frameT),1, frameColor);


	return frame;
}


void InGameHUD::fillHealthBar() const {
    float y = 20;
    float endX = 300 * player_.hp_ / 100;
	if (player_.hp_ <= 30){
		drawLine(Vec2(11, y), Vec2( endX, y), 19, D3DCOLOR_ARGB(0xff,0xff,0x11,0x11));
	}
	else{
		drawLine(Vec2(11, y), Vec2( endX, y), 19, D3DCOLOR_ARGB(0xff,0x11,0xff,0x11));
	}
}

void InGameHUD::drawHealthBar() const {
    drawRect(10, 10, 300, 30, D3DCOLOR_ARGB(0xff,0xff,0xff,0xff));
}


void InGameHUD::drawLine(Vec2 start, Vec2 end, float width, D3DCOLOR lineColor) const {
    LPD3DXLINE gLine;
    D3DXCreateLine(D3DManager::Get()->GetD3DDevice(), &gLine);
    gLine->SetWidth(width);
    gLine->SetAntialias(false);
    gLine->SetGLLines(false);

    const int pointCount = 2;
    D3DXVECTOR2 Line[pointCount];
    int numSides(20);

    Line[0].x = start.x;
    Line[0].y = start.y;

    Line[1].x = end.x;
    Line[1].y = end.y;

    gLine->Begin();

    gLine->Draw(Line,pointCount,lineColor);

    gLine->End();
    gLine->Release();
}

void InGameHUD::drawRect(float left, float top, float right, float bottom, D3DCOLOR lineColor) const {
    LPD3DXLINE gLine;
    D3DXCreateLine(D3DManager::Get()->GetD3DDevice(), &gLine);
    gLine->SetWidth(1);
    gLine->SetAntialias(false);
    gLine->SetGLLines(false);

    const int pointCount = 5;
    D3DXVECTOR2 Line[pointCount];
    int numSides(20);

    Line[0].x = left;
    Line[0].y = top;

    Line[1].x = right;
    Line[1].y = top;

    Line[2].x = right;
    Line[2].y = bottom;

    Line[3].x = left;
    Line[3].y = bottom;

    Line[4].x = left;
    Line[4].y = top;

    gLine->Begin();

    gLine->Draw(Line,pointCount,lineColor);



    gLine->End();
    gLine->Release();
}