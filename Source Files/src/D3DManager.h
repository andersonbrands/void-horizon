// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef D3DMANAGER_H
#define D3DMANAGER_H

#include <d3dx9.h>		// Direct 3D library (for all Direct 3D functions).
#include <vector>
#include <DxErr.h>
#include <string>

//handy macro to save time checking the return value from D3D functions
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
	if(FAILED(hr))                                 \
	{                                              \
		DXTraceW(__FILE__, __LINE__, hr, L#x, TRUE); \
	}                                              \
}
#endif
#else
#ifndef HR
#define HR(x) x;
#endif
#endif


namespace D3D
{



/*
Wrap up D3D work we keep repeating
*/
class D3DManager 
{
public:
	D3DManager(HINSTANCE hinst, const std::string &winName);
	~D3DManager();

	//accessors
	static D3DManager *Get();
	HWND GetHWnd() { return mhWnd; }
	HINSTANCE GetHinst() { return mhinst; }
	LPDIRECT3D9 GetD3D() { return mpD3D; }
	LPDIRECT3DDEVICE9 GetD3DDevice() { return mpd3dDevice; }
	
	//wrap any rendering we do
	void BeginScene();
	void EndScene();
	//scale or scroll a texture, perfect for scrolling backgrounds
	void SetTextureMatrix(bool enable, float tx=0, float ty=0, float sx=1, float sy=1, float rot=0);
	//keep windows happy
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	//check everything is set up OK
	bool GetIsInitialised() const { return mpD3D!=nullptr && mpOnLostDevice && mpOnResetDevice; }
	//get the window open at the beginning
	void InitMainWindow();
	//screen size
	int GetBufferWidth() const { return md3dParams.BackBufferWidth; }
	int GetBufferHeight() const { return md3dParams.BackBufferHeight; }
	//callbacks so D3D can tell us if the device is lost so we can do something about it
	//the last parameter is an optional context that will be passed to the first two functions
	void SetResetAndLost( void (*pOnLostDevice)(void *p), void (*pOnResetDevice)(void *p), void *pMe=nullptr ) { 
		mpOnLostDevice=pOnLostDevice; mpOnResetDevice=pOnResetDevice; mpResetAndLost=pMe; }
	void OnLost();
	void OnReset();
	//are we inbetween switch windowed/full screen i.e. in limbo with no way to render
	bool GetIsDeviceLost();
	//fixed colour background
	void SetBackColour(float r, float g, float b) {
		mbackColour = D3DXCOLOR(r,g,b,1.f);
	}

	//if we don't have focus then we shouldn't be doing anything
	bool GetIsPaused() const { return mAppPaused; }	

private:

	static D3DManager *spD3DMgr;
	HWND mhWnd;
	HINSTANCE mhinst;
	LPDIRECT3D9 mpD3D;				// Used to create the D3DDevice
	LPDIRECT3DDEVICE9 mpd3dDevice;	// The rendering device
	D3DXCOLOR mbackColour;
	D3DPRESENT_PARAMETERS md3dParams;	//everything we need to setup D3D
	std::string mWinName;		//window name
	
	//pointers to callback functions
	void (*mpOnLostDevice)(void *p);	
	void (*mpOnResetDevice)(void *p);
	void *mpResetAndLost;

	int mLastWindowedWidth, mLastWindowedHeight;	//remember the window size if we drop back from fullscreen
	
	bool mAppPaused;	//lost focus so don't do anything

	//grunt work getting D3D started
	void SetupD3D(D3DDEVTYPE devType, DWORD requestedVP);

	//switch between full screen and windowed
	void EnableFullScreenMode(bool enable);
};

}

inline D3D::D3DManager *GetD3DManager()
{
	return D3D::D3DManager::Get();
}

#endif