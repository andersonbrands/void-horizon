// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <dsound.h>


/*
DirectSound is more low level than DirectMusic, but the easier
to use DireectMusic was deprecated and only works with August 2007
SDK or earlier. To get streaming music playback (i.e. playing a BIG
music file) you need to use something oggvorbis with DiretSound 
otherwise it will just load it all in!
*/
class Sound
{
private:
	//see msdn - this is what a wav file looks like
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	enum { MAX_SOUNDS=50,			//how many we can load at once
			NULL_HDL=2147483647		//an uninitialised sound handle
	};	
	Sound(HWND hWnd);
	~Sound();
	static Sound *Get();	//singleton

	/*
	load in a wav file from disk and get a handle to it
	NOTE - wavs only, must be 16bit PCM and make sure you clear 
	out the header in audacity on save or the extra data (date, name, etc)
	will through out the header reading and make the load fail
	*/
	bool LoadWaveFile(const char *fileName, int &hdl);
	//when done with a wav file, not usually needed
	void ShutdownWaveFile(int hdl);
	//play one you loaded, volume is logarithmic decibels so scale not linear
	//once a sound is loaded you can play back many instances of it, even if they overlap
	bool PlayWaveFile(int hdl, bool loop=false, float volume=1.f);

	//these functions will affect all instances of a sound effect based on its handle
	//i.e. there may be 20 "whoop.wav" sounds playing and these functions would apply to all of them
	//stop it playing early or if its looping
	bool StopWaveFile(int hdl);
	//change the frequency is like changing the pitch
	bool SetFrequency(int hdl, LONG freq);
	//check if its still playing
	bool GetIsPlaying(int hdl);
	bool SetVolume( int hdl, float volume);

private:
	Sound(const Sound&) {};
	bool InitialiseDirectSound(HWND);
	void ShutdownDirectSound();

	IDirectSound8* m_DirectSound;		//main handle DSound
	IDirectSoundBuffer* m_primaryBuffer;	//all our sounds are mixed into this one primary buffer sent to the speakers
	IDirectSoundBuffer8* m_buffers[MAX_SOUNDS];	//all our wav files loaded into memory - so keep them small

	static Sound *spSound;	//singleton
};

#endif