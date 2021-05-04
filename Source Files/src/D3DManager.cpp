// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "D3DManager.h"
#include "Error.h"

namespace D3D
{



D3DManager *D3DManager::spD3DMgr=nullptr;

//keep windows happy, but don't let us take over until we are setup properly
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Don't start processing messages until the application has been created.
	if( D3DManager::Get()->GetIsInitialised() )
		return D3DManager::Get()->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}


D3DManager *D3DManager::Get() 
{
	MPOD_ASSERT(spD3DMgr);
	return spD3DMgr;
}

void D3DManager::SetupD3D(D3DDEVTYPE devType, DWORD requestedVP)
{

	// Create the D3D object.
	if (nullptr == (mpD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MPOD_ASSERT_MSG(false,"Failed to create D3D");
		return;
	}

	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.

	D3DDISPLAYMODE mode;
	mpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(mpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, devType, mode.Format, mode.Format, true));
	HR(mpD3D->CheckDeviceType(D3DADAPTER_DEFAULT, devType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	// Step 3: Check for requested vertex processing and pure device.

	D3DCAPS9 caps;
	HR(mpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, devType, &caps));

	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= requestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;


	// Set up the structure used to create the D3DDevice
	ZeroMemory(&md3dParams, sizeof(md3dParams));
	md3dParams.BackBufferWidth            = 0; 
	md3dParams.BackBufferHeight           = 0;
	md3dParams.BackBufferFormat           = D3DFMT_UNKNOWN;
	md3dParams.BackBufferCount            = 1;
	md3dParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
	md3dParams.MultiSampleQuality         = 0;
	md3dParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	md3dParams.hDeviceWindow              = mhWnd;
	md3dParams.Windowed                   = true;
	md3dParams.EnableAutoDepthStencil     = true; 
	md3dParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
	md3dParams.Flags                      = 0;
	md3dParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	md3dParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


	// Create the D3DDevice
	HR(mpD3D -> CreateDevice(
		D3DADAPTER_DEFAULT, 
		devType, 
		mhWnd,
		devBehaviorFlags,
		&md3dParams, 
		&mpd3dDevice));

	// Enable the Z buffer, since we're dealing with 3D geometry.
	mpd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

D3DManager::D3DManager(HINSTANCE hinst, const std::string &winName) 
	: mhWnd(0), mhinst(hinst), mpD3D(nullptr), mpd3dDevice(nullptr), mbackColour(0,0,1,1), 
	mAppPaused(false), mpOnLostDevice(nullptr), mpOnResetDevice(nullptr), mpResetAndLost(nullptr),
	mWinName(winName)
{
	MPOD_ASSERT(!spD3DMgr);
	spD3DMgr = this;
	InitMainWindow();
	SetupD3D(D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
}


D3DManager::~D3DManager()
{
	MPOD_ASSERT(spD3DMgr);
	if (mpd3dDevice != nullptr)	 
	{
		mpd3dDevice -> Release();
		mpd3dDevice = nullptr;
	}
	if (mpD3D != nullptr)			 
	{
		mpD3D -> Release();
		mpD3D = nullptr;
	}
	UnregisterClass("D3DWndClassName", mhinst);
	spD3DMgr=nullptr;
}


void D3DManager::BeginScene()
{
	MPOD_ASSERT_MSG(GetIsInitialised(),"D3D not initialised");
	// Clear the back buffer, also clear the Z buffer at the same time.
	mpd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, mbackColour, 1.0f, 0);

	// Begin the scene
	if (FAILED(mpd3dDevice->BeginScene()))
		MPOD_ASSERT_MSG(false, "Begin scene failed");
}

void D3DManager::EndScene()
{
	mpd3dDevice->EndScene();
	// Present the back buffer to the display.
	mpd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void D3DManager::SetTextureMatrix(bool enable, float tx, float ty, float sx, float sy, float rot)
{
	if(!enable)
	{
		mpd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		return;
	}

	mpd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	//although its a 3D matrix, textures are 2D so we only mess with the x/y part of it
	D3DXMATRIX TMatrix;
	float cosRot=cosf(rot), sinRot=sinf(rot);
	TMatrix._11=sx * cosRot;	TMatrix._12=-sinRot;		TMatrix._13=0;
	TMatrix._21=sinRot;			TMatrix._22=sy * cosRot;	TMatrix._23=0;
	TMatrix._31=tx;				TMatrix._32=ty;				TMatrix._33=1;
	mpd3dDevice->SetTransform(D3DTS_TEXTURE0, &TMatrix );
}

void D3DManager::InitMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhinst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Default to a window with a client area rectangle of 600x600.

	RECT R = {0, 0, 1280, 720};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mhWnd = CreateWindow("D3DWndClassName", mWinName.c_str(), 
		WS_OVERLAPPEDWINDOW, 100, 100, R.right, R.bottom, 
		0, 0, mhinst, 0); 

	if( !mhWnd )
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
}

//process messages from windows
LRESULT D3DManager::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Is the application in a minimized or maximized state?
	static bool minOrMaxed = false;
	if(mpd3dDevice)
		MPOD_ASSERT(mpOnLostDevice && mpOnResetDevice);
	RECT clientRect = {0, 0, 0, 0};
	switch( msg )
	{

		// WM_ACTIVE is sent when the window is activated or deactivated.
		// We pause the game when the main window is deactivated and 
		// unpause it when it becomes active.
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
			mAppPaused = true;
		else
			mAppPaused = false;
		return 0;


		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		if( mpd3dDevice )
		{
			md3dParams.BackBufferWidth  = LOWORD(lParam);
			md3dParams.BackBufferHeight = HIWORD(lParam);

			if( wParam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				minOrMaxed = true;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				minOrMaxed = true;
				MPOD_ASSERT(mpOnLostDevice && mpOnResetDevice);
				mpOnLostDevice(mpResetAndLost);
				if(FAILED(mpd3dDevice->Reset(&md3dParams)))
					MPOD_ASSERT(false);
				mpOnResetDevice(mpResetAndLost);
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if( wParam == SIZE_RESTORED )
			{
				mAppPaused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if( minOrMaxed && md3dParams.Windowed )
				{
					mpOnLostDevice(mpResetAndLost);
					if(FAILED(mpd3dDevice->Reset(&md3dParams)))
						MPOD_ASSERT(false);
					mpOnResetDevice(mpResetAndLost);
				}
				else
				{
					// No, which implies the user is resizing by dragging
					// the resize bars.  However, we do not reset the device
					// here because as the user continuously drags the resize
					// bars, a stream of WM_SIZE messages is sent to the window,
					// and it would be pointless (and slow) to reset for each
					// WM_SIZE message received from dragging the resize bars.
					// So instead, we reset after the user is done resizing the
					// window and releases the resize bars, which sends a
					// WM_EXITSIZEMOVE message.
				}
				minOrMaxed = false;
			}
		}
		return 0;


		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		GetClientRect(mhWnd, &clientRect);
		md3dParams.BackBufferWidth  = clientRect.right;
		md3dParams.BackBufferHeight = clientRect.bottom;
		mpOnLostDevice(mpResetAndLost);
		if(FAILED(mpd3dDevice->Reset(&md3dParams)))
			MPOD_ASSERT(false);
		mpOnResetDevice(mpResetAndLost);
		return 0;

		// WM_CLOSE is sent when the user presses the 'X' button in the
		// caption bar menu.
	case WM_CLOSE:
		DestroyWindow(mhWnd);
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		//toggle full screen
	case WM_KEYDOWN:
		if( wParam == VK_F12 )
			EnableFullScreenMode((md3dParams.Windowed)?true:false);
		return 0;
	}
	return DefWindowProc(mhWnd, msg, wParam, lParam);
}

void D3DManager::EnableFullScreenMode(bool enable)
{
	// Switch to fullscreen mode.
	if( enable )
	{
		// Are we already in fullscreen mode?
		if( !md3dParams.Windowed ) 
			return;
		mLastWindowedHeight=GetBufferHeight();
		mLastWindowedWidth=GetBufferWidth();
		int width  = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		//ZeroMemory(&md3dParams, sizeof(md3dParams));
		md3dParams.BackBufferFormat = D3DFMT_X8R8G8B8;
		md3dParams.BackBufferWidth  = width;
		md3dParams.BackBufferHeight = height;
		md3dParams.Windowed         = false;

		// Change the window style to a more fullscreen friendly style.
		SetWindowLongPtr(mhWnd, GWL_STYLE, WS_POPUP);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);	
	}
	// Switch to windowed mode.
	else
	{
		// Are we already in windowed mode?
		if( md3dParams.Windowed ) 
			return;

		RECT R = {0, 0, mLastWindowedWidth, mLastWindowedHeight};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		//ZeroMemory(&md3dParams, sizeof(md3dParams));
		md3dParams.BackBufferFormat = D3DFMT_UNKNOWN;
		md3dParams.BackBufferWidth  = mLastWindowedWidth;
		md3dParams.BackBufferHeight = mLastWindowedHeight;
		md3dParams.Windowed         = true;

		// Change the window style to a more windowed friendly style.
		SetWindowLongPtr(mhWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// If we call SetWindowLongPtr, MSDN states that we need to call
		// SetWindowPos for the change to take effect.  In addition, we 
		// need to call this function anyway to update the window dimensions.
		SetWindowPos(mhWnd, HWND_NOTOPMOST, 100, 100, R.right, R.bottom, SWP_SHOWWINDOW);
	}

	// Reset the device with the changes.
	mpOnLostDevice(mpResetAndLost);
	GetIsDeviceLost();
	HR(mpd3dDevice->Reset(&md3dParams));
	mpOnResetDevice(mpResetAndLost);
}

bool D3DManager::GetIsDeviceLost()
{
	// Get the state of the graphics device.
	HRESULT hr = mpd3dDevice->TestCooperativeLevel();

	// If the device is lost and cannot be reset yet then
	// sleep for a bit and we'll try again on the next 
	// message loop cycle.
	if( hr == D3DERR_DEVICELOST )
	{
		Sleep(20);
		return true;
	}
	// Driver error, exit.
	else if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, "Internal Driver Error...Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// The device is lost but we can reset and restore it.
	else if( hr == D3DERR_DEVICENOTRESET )
	{
		mpOnLostDevice(mpResetAndLost);
		HR(mpd3dDevice->Reset(&md3dParams));
		mpOnResetDevice(mpResetAndLost);
		return false;
	}
	else
		return false;
}


}