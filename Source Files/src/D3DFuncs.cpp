// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "D3DFuncs.h"
#include "D3DManager.h"
#include "Error.h"

namespace D3D
{

void SetupVNTGeometry( CUSTOMVERTEX* pV, int index, 
	float px, float py, float pz, 
	float nx, float ny, float nz,
	float u, float v)
{
	pV[index].position.x = px;	// Vertex co-ordinate.
	pV[index].position.y = py;
	pV[index].position.z = pz;
	pV[index].normal.x = nx;	// Vertex normal.
	pV[index].normal.y = ny;
	pV[index].normal.z = nz;
	pV[index].u = u;
	pV[index].v = v;
}


void SetupViewMatrices(float x, float y, float z, float tx, float ty, float tz)
{
	// Set up the view matrix.
	// This defines which way the 'camera' will look at, and which way is up.
	D3DXVECTOR3 vCamera(x, y, z);
	D3DXVECTOR3 vLookat(tx,ty,tz);
	D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vCamera, &vLookat, &vUpVector);
	D3DManager::Get()->GetD3DDevice()->SetTransform(D3DTS_VIEW, &matView);

	// Set up the projection matrix.
	// This transforms 2D geometry into a 3D space.
	float w = (float)D3DManager::Get()->GetBufferWidth();
	float h = (float)D3DManager::Get()->GetBufferHeight();
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, w/h, 1.0f, 400.0f);
	D3DManager::Get()->GetD3DDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CreateMaterial(float r, float g, float b, D3DMATERIAL9 &Mtl)
{
	// Define a material.
	// Reflects only diffuse colour.
	ZeroMemory(&Mtl, sizeof(D3DMATERIAL9));
	Mtl.Diffuse.r = r;
	Mtl.Diffuse.g = g;
	Mtl.Diffuse.b = b;
	Mtl.Diffuse.a = 1.0f;
}

void SetupMaterial(float r, float g, float b)
{
	D3DMATERIAL9 Mtl;
	CreateMaterial(r,g,b,Mtl);
	// Set material as current material
	D3DManager::Get()->GetD3DDevice()->SetMaterial(&Mtl);
}

void SetupAmbient( float r, float g, float b )
{
	MPOD_ASSERT(r>=0 && r<=1 && g>=0 && g<=1 && b>=0 && b<=1);
	D3DManager::Get()->GetD3DDevice()->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB((int)(r*255.f),(int)(g*255.f),(int)(b*255.f)) );
}

void SetupDirectionalLight( int lightIdx, bool enable, 
					 float diffR/*=1*/, float diffG/*=1*/, float diffB/*=1*/, 
					 const D3DXVECTOR3 &direction/*=D3DXVECTOR3(0,0,0)*/, 
					 float range/*=1000.f*/, float atten1/*=0.05f*/ )
{
	LPDIRECT3DDEVICE9 d3d = D3DManager::Get()->GetD3DDevice();

	if(!enable)
	{
		d3d->LightEnable(lightIdx, FALSE);
		return;
	}

	// Define a directional light.
	D3DLIGHT9 SampleLight;
	ZeroMemory(&SampleLight, sizeof(D3DLIGHT9));
	SampleLight.Type = D3DLIGHT_DIRECTIONAL;

	// Light possesses only a diffuse colour.
	SampleLight.Diffuse.r = diffR;
	SampleLight.Diffuse.g = diffG;
	SampleLight.Diffuse.b = diffB;

	// Light points directly down and left.
	SampleLight.Direction=direction; 

	SampleLight.Range = range;
	SampleLight.Attenuation0 = 0;
	SampleLight.Attenuation1 = atten1;
	SampleLight.Attenuation2 = 0;

	// Select and enable the light (with index 0).
	d3d->SetLight(lightIdx, &SampleLight);
	d3d->LightEnable(lightIdx, TRUE);
}


void SetupPixelFog(DWORD color, DWORD mode, float start, float end)
{
	//float Start   = 50;// 0.5f;    // For linear mode
	//float End     = 100;//0.8f;
	float density = 0.66f;   // For exponential modes
	LPDIRECT3DDEVICE9 d3d = D3DManager::Get()->GetD3DDevice();

	// Enable fog blending.
	d3d->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// Set the fog color.
	d3d->SetRenderState(D3DRS_FOGCOLOR, color);

	// Set fog parameters.
	d3d->SetRenderState(D3DRS_FOGTABLEMODE, mode);
	if( mode == D3DFOG_LINEAR )
	{
		d3d->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
		d3d->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&end));
	}
	else
	{
		d3d->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density));
	}
}




}