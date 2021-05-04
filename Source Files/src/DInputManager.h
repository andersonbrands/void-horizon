// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef DINPUTMANAGER_H
#define DINPUTMANAGER_H

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"



/*
Wrap DInput to make it easier to deal with
*/
class DInputManager 
{
public:
	DInputManager();
	~DInputManager();
	static DInputManager *Get();
	void Update();	//call this regularly
	bool KeyDown(unsigned char dikCode) {
		return (mBuffer[dikCode] & 0x80) ? true : false;
	}
	//have they pressed AND then let go?
	bool Pressed(unsigned char dikCode) {
		return !KeyDown(dikCode) && (mLastBuffer[dikCode]&0x80);
	}

private:
	static DInputManager *spDIMgr;
	static const int BUFFER_SIZE = 256;			//keyboards are fixed size
	LPDIRECTINPUT8 mpDI;						//main device context
	LPDIRECTINPUTDEVICE8 mpDIKeyboardDevice;	//keyboard interface
	char mBuffer[BUFFER_SIZE];					
	char mLastBuffer[BUFFER_SIZE];				//remember what was down previously for press+release
};

#endif