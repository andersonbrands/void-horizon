// by Mark Featherstone (C) 2013 All Rights Reserved.
#include <stdio.h>


#include "Sound.h"
#include "Error.h"


Sound *Sound::spSound(nullptr);

Sound::Sound(HWND hWnd)
{
	MPOD_ASSERT(!spSound);
	spSound=this;
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	for(int i(0);i<MAX_SOUNDS;++i)
		m_buffers[i]=nullptr;
	InitialiseDirectSound(hWnd);
}

Sound::~Sound()
{
	spSound=nullptr;
	// Release the secondary buffers.
	for(int i(0);i<MAX_SOUNDS;++i)
		ShutdownWaveFile(i);

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();
}


bool Sound::InitialiseDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(nullptr, &m_DirectSound, nullptr);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = nullptr;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, nullptr);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void Sound::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if(m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if(m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}


bool Sound::LoadWaveFile(const char* filename, int &hdl)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;


	IDirectSoundBuffer8** secondaryBuffer=0;
	{
		int i(0);
		while(i<MAX_SOUNDS && !secondaryBuffer)
		{
			if(!m_buffers[i])
			{
				secondaryBuffer=&m_buffers[i];
				hdl=i;
			}
			++i;
		}
	}
	if(!secondaryBuffer)
		return false;
	
	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
	   (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
	   (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
	   (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}


	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header.
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	   (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveFileHeader.sampleRate;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = waveFileHeader.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
	if(FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, nullptr, 0, 0);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, nullptr, 0);
	if(FAILED(result))
	{
		return false;
	}
	
	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = 0;

	return true;
}


void Sound::ShutdownWaveFile(int hdl)
{
	// Release the secondary sound buffer.
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS);
	if(m_buffers[hdl])
	{
		m_buffers[hdl]->Release();
		m_buffers[hdl] = 0;
	}

	return;
}


bool Sound::PlayWaveFile(int hdl, bool loop, float volume)
{
	HRESULT result;
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS && m_buffers[hdl]);
	IDirectSoundBuffer8 *pBuffer(m_buffers[hdl]);

	// Set position at the beginning of the sound buffer.
	result = pBuffer->SetCurrentPosition(0);
	if(FAILED(result))
	{
		return false;
	}

	SetVolume(hdl,volume);

	// Play the contents of the secondary sound buffer.
	DWORD flags(0);
	if(loop)
		flags|=DSBPLAY_LOOPING;
	result = pBuffer->Play(0, 0, flags);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

Sound * Sound::Get()
{
	MPOD_ASSERT(spSound);
	return spSound;
}

bool Sound::StopWaveFile(int hdl )
{
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS && m_buffers[hdl]);
	if(FAILED(m_buffers[hdl]->Stop()))
		return false;
	return true;
}

bool Sound::GetIsPlaying( int hdl )
{
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS && m_buffers[hdl]);
	DWORD status;
	if(FAILED(m_buffers[hdl]->GetStatus(&status)))
		MPOD_ASSERT(0);
	return status&DSBSTATUS_PLAYING;
}

bool Sound::SetFrequency( int hdl, LONG freq )
{
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS && m_buffers[hdl]);
	if(FAILED(m_buffers[hdl]->SetFrequency(freq)))
		return false;
	return true;
}

#include <math.h>

bool Sound::SetVolume( int hdl, float volume )
{
	MPOD_ASSERT(hdl>=0 && hdl<MAX_SOUNDS && m_buffers[hdl]);
	if(volume>1.f) volume=1.f;
	float db = DSBVOLUME_MIN * 0.5f * log(1.f+(1.f-volume));
	if( db<DSBVOLUME_MIN) db=DSBVOLUME_MIN;

	if(FAILED(m_buffers[hdl]->SetVolume((LONG)db)))
		return false;
	return true;
}
