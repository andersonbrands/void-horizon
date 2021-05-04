
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "mmsystem.h"

#include "D3DManager.h"
#include "DInputManager.h"
#include "Mesh.h"
#include "Error.h"
#include "Sound.h"
#include "GameObject.h"
#include "Game.h"

using namespace D3D;

// updates per second
#define UPS 30

// throttle frame time at 10 fps (i.e. the game will slow down rather
// than having very low frame rate)
#define MAX_UPDATES 3

//if true we need to exit immediately
bool g_okToQuit = false;

//if we were using some game specific 3rd party, set it up and shut it down here
void InitExtModules()
{
}

void TerminateExtModules()
{
}

/*
Create and start up all the systems we made to run the game
*/
void InitAppModules(HINSTANCE hinst)
{

	//each of these is a manager so there is only one (singleton)
	new D3DManager(hinst, "Void Horizon");
	new DInputManager;
	new Sound(D3DManager::Get()->GetHWnd());
	new SpriteManager;
	new Game;
}


void TerminateAppModules()
{
	// Delete the managers (singleton classes) and they'll
	//delete all their children (game objects, etc)
	delete Game::Get();
	delete Sound::Get();
	delete DInputManager::Get();
	delete D3DManager::Get();
	delete GetSpriteManager();
}


//count up at desired frame rate
int GetUpdateFrame()
{
	return (int)(timeGetTime() / (1000/UPS));
}

//check for exit request i.e. task switch, press cross button, etc
bool CheckForExit(MSG msg)
{
	if(g_okToQuit)
		return true;

	if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if(msg.message == WM_QUIT)
		g_okToQuit = true;

	return g_okToQuit;
}

//our main game loop
void MainLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	int currentUpdate = GetUpdateFrame();
	int nextUpdate = currentUpdate;
	bool wasPaused=false;



	// to exit correctly, applications should poll for quit requests
	while(!CheckForExit(msg))
	{
		// run logic at a fixed frame rate (defined by UPS)
		// block until the next frame (don't render unless at
		// least one update has occurred)
		while(!CheckForExit(msg))
		{
			nextUpdate = GetUpdateFrame();
			if( nextUpdate != currentUpdate )
				break;
			Sleep(1);
		}


		DInputManager::Get()->Update();
		if(!CheckForExit(msg) && !D3DManager::Get()->GetIsDeviceLost() && !D3DManager::Get()->GetIsPaused())
		{
			// execute update steps
			int frames = nextUpdate - currentUpdate;
			frames = (MAX_UPDATES<frames) ? MAX_UPDATES : frames;
			while(frames--)
			{
				// Main update
				Game::Get()->Update(1.f/UPS);
			}
			currentUpdate = nextUpdate;

			// Render 
			Game::Get()->Render(1.f/UPS);
		}

		Sleep(0);

	}
}



//app entry point
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	// Enable run-time memory check for debug builds.
	// Just helps us spot a few more errors
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif


	// Initialise everything
	InitExtModules();
	InitAppModules(hInst);
	srand(timeGetTime());

	//sit in here until we exit
	MainLoop();

	// Terminate everything
	TerminateAppModules();
	TerminateExtModules();

	return 0;
}
