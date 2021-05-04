// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "DInputManager.h"
#include "Error.h"
#include "D3DManager.h"

using namespace D3D;

DInputManager *DInputManager::spDIMgr=nullptr;


DInputManager::DInputManager()
{
	spDIMgr=this;
	// Create the DirectInput object. 
	if(FAILED(DirectInput8Create(D3DManager::Get()->GetHinst(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpDI, nullptr)))
		MPOD_ASSERT_MSG(false,"DInput8Create failed");

	// Retrieve a pointer to an IDirectInputDevice8 interface.
	if(FAILED(mpDI->CreateDevice(GUID_SysKeyboard, &mpDIKeyboardDevice, nullptr)))
		MPOD_ASSERT_MSG(false, "DInput CreateDevice failed");

	// Now that you have an IDirectInputDevice8 interface, get 
	// ready to use it. 

	// Set the data format using the predefined keyboard data format.
	if(FAILED(mpDIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
		MPOD_ASSERT_MSG(false,"Failed to set data format for keyboard");

	// Set the cooperative level 
	if(FAILED(mpDIKeyboardDevice->SetCooperativeLevel(D3DManager::Get()->GetHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		MPOD_ASSERT_MSG(false,"Failed to set cooperative level of keyboard");

	mpDIKeyboardDevice->Acquire();

	ZeroMemory(&mBuffer, BUFFER_SIZE);
	ZeroMemory(&mLastBuffer, BUFFER_SIZE);
}	

DInputManager::~DInputManager()
{ 
	if (mpDI) 
	{ 
		if (mpDIKeyboardDevice) 
		{ 
			// Always unacquire device before calling Release(). 
			mpDIKeyboardDevice -> Unacquire(); 
			mpDIKeyboardDevice -> Release();
			mpDIKeyboardDevice = nullptr; 
		} 
		mpDI -> Release();
		mpDI = nullptr; 
	} 
	spDIMgr=nullptr;
} 



void DInputManager::Update()
{ 
	HRESULT hr;

	//remember what was down, so we know when something is pressed and released
	memcpy(mLastBuffer, mBuffer, BUFFER_SIZE);

	// Clear the buffer for keyboard data - just in case.
	ZeroMemory(&mBuffer, BUFFER_SIZE);

	hr = mpDIKeyboardDevice->GetDeviceState(sizeof(mBuffer),(LPVOID)&mBuffer); 
	if FAILED(hr) 
	{ 
		// If this failed, the device has probably been lost.
		// Check for (hr == DIERR_INPUTLOST) and attempt to reacquire it here.
		hr = mpDIKeyboardDevice->Acquire();
		while (hr == DIERR_INPUTLOST) 
			hr = mpDIKeyboardDevice->Acquire();

		hr = mpDIKeyboardDevice->GetDeviceState(sizeof(mBuffer),(LPVOID)&mBuffer); 
	} 
}

DInputManager * DInputManager::Get()
{
	MPOD_ASSERT(spDIMgr);
	return spDIMgr;
}
