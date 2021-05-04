// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "Font.h"
#include "Error.h"

Font::Font()
{
	m_pFont = nullptr; 
}

/* 
pDevice - D3D Device for D3DXCreateFont call
faceName - Font name
size - Font size
bold - Bold if TRUE
italic - Italicized if TRUE
*/
bool Font::Init( LPDIRECT3DDEVICE9 pDevice, const char* faceName, int size, bool bold, bool italic )
{
    Release();
    HRESULT hr = 0;
    hr = D3DXCreateFont( pDevice, -size, 0, bold ? 800 : 0, 1, italic, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, faceName, &m_pFont );
    if ( FAILED( hr ) )
    {
		MPOD_ASSERT_MSG(false, "D3DXCreateFont() failed.");
        return false;
    }
    return true;
}

/* 
text - Text to print
xPosition - X position in window coordinates
yPosition - Y position in window coordinates
color - Color of the text.
sprite - Sprite for batch printing
textBoxWidth - Width to constrain text in
textBoxHeight - Height to constrain text in
format - FONTALIGNMENT value.
dimensionOnly - doesn't render anything, but gives you dimensions if it did
Returns - RECT the rectangular size that would be rendered into, does if dimensionsOnly is true
*/
RECT Font::Print( const char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite, 
    int textBoxWidth, int textBoxHeight, FontAlignment::Enum alignment, bool dimensionsOnly ) const
{
	RECT rect = {0,0,0,0};
    if ( !m_pFont ) 
    {
        return rect;
    }
    DWORD format = DT_EXPANDTABS;
    if ( textBoxWidth == 0 )
    {
        format |= DT_NOCLIP;
    }
    else
    {
        format |= DT_WORDBREAK;
        switch ( alignment )
        {
		case FontAlignment::FA_LEFT:
            format |= DT_LEFT;
            break;
        case FontAlignment::FA_CENTRE:
            format |= DT_CENTER;
            break;
        case FontAlignment::FA_RIGHT:
            format |= DT_RIGHT;
            break;
        case FontAlignment::FA_TOPRIGHT:
            format |= DT_RIGHT | DT_TOP;
            break;
        case FontAlignment::FA_BOTTOMRIGHT:
            format |= DT_RIGHT | DT_BOTTOM;
            break;
        case FontAlignment::FA_TOPLEFT:
            format |= DT_LEFT | DT_TOP;
            break;
        case FontAlignment::FA_BOTTOMLEFT:
            format |= DT_LEFT | DT_BOTTOM;
            break;
        }
        if ( textBoxHeight == 0 )
        {
            // A width is specified, but not a height.
            // Make it seem like height is infinite
            textBoxHeight = 2000;
        }
    }
	if(dimensionsOnly)
		format |= DT_CALCRECT;

	rect.left=xPosition;
	rect.top=yPosition;
	rect.right=xPosition + textBoxWidth;
	rect.bottom=yPosition + textBoxHeight;
    m_pFont->DrawText( sprite, text, -1, &rect, format, color );

	return rect;
}

void Font::OnLostDevice()
{
    if ( m_pFont )
    {
        m_pFont->OnLostDevice();
    }
}

void Font::OnResetDevice()
{
    if ( m_pFont )
    {
        m_pFont->OnResetDevice();
    }
}

void Font::Release()
{
	//finished with font so release it
    if( m_pFont )
		m_pFont->Release();
	m_pFont=NULL;
}

