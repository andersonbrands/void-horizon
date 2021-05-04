// by Mark Featherstone (C) 2013 All Rights Reserved.

#ifndef FONT_H
#define FONT_H
#include <d3d9.h>
#include <d3dx9core.h>

// Font alignment
namespace FontAlignment { typedef enum 
{ 
	FA_LEFT, 
	FA_CENTRE, 
	FA_RIGHT, 
	FA_TOPRIGHT, 
	FA_TOPLEFT, 
	FA_BOTTOMRIGHT, 
	FA_BOTTOMLEFT 
} Enum; }

/*
Wrap a D3DX font to make printing simpler
*/
class Font
{
public:
    Font();
    ~Font() { Release(); }
    bool Init( LPDIRECT3DDEVICE9 pDevice, const char* faceName, int size, bool bold = FALSE, bool italic = FALSE );
    RECT Print( const char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite = NULL, 
        int textBoxWidth = 0, int textBoxHeight = 0, FontAlignment::Enum alignment = FontAlignment::FA_LEFT, bool dimensionsOnly=false ) const;
    //fonts always get lost if we switch from window to fullscreen, etc
	void OnLostDevice();
    void OnResetDevice();
    void Release();
private:
    LPD3DXFONT m_pFont;		//our D3DX font 
};

#endif