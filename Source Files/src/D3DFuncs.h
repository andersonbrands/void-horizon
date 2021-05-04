// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef D3DFUNCS_H
#define D3DFUNCS_H

#include <d3dx9.h>

namespace D3D
{
// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;	// Position
	D3DXVECTOR3 normal;		// Vertex normal
	float u, v;       // texture coordinate
};

// The structure of a vertex in our vertex buffer...
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

void SetupVNTGeometry( CUSTOMVERTEX* pV, int index, 
	float px, float py, float pz, 
	float nx, float ny, float nz,
	float u, float v);

// Set up the view - the view and projection matrices.
void SetupViewMatrices(float x, float y, float z, float tx, float ty, float tz);
//create a material that you can hang on to
void CreateMaterial(float r, float g, float b, D3DMATERIAL9 &Mtl);
//setup and apply a material immediately
void SetupMaterial(float r, float g, float b);

//constant light in one direction like the sun
void SetupDirectionalLight(int lightIdx, bool enable, 
					 float diffR=1, float diffG=1, float diffB=1,
					 const D3DXVECTOR3 &position=D3DXVECTOR3(0,0,0),
					 float range=1000.f, float atten1=0.05f);
//ambient light is everywhere and has no direction
void SetupAmbient(float r, float g, float b);
//useful to hide things in the distance i.e. objects approaching the camera
void SetupPixelFog(DWORD color, DWORD mode, float start, float end);




}

#endif