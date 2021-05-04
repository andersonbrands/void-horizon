#include "Utils.h"

#include <assert.h>
#include <time.h>
#include <cstdlib>


int GetRandom( int min, int max )
{
	int range = (max-min)+1;
	assert(max>min && range>1);
	int res = min + (int)(((float)rand() / RAND_MAX) * range);
	return res;
}

float GetRandom( float min, float max )
{
	float range = max-min;
	assert(max>min && range>0);
	float res = min + ((float)rand() / RAND_MAX) * range;
	return res;
}

void SeedRandom( unsigned int seed/*=MAX_UINT*/ )
{
	if(seed==UINT_MAX)
		srand((unsigned)time(NULL));
	else
		srand(seed);
}

void SetTransform(LPDIRECT3DDEVICE9 pD3D,
					float x, float y, float z, /*position */ 
					float scaleX/*=1.f*/, float scaleY/*=1.f*/, float scaleZ/*=1.f*/,  /*scale to apply */
					float rotX/*=0*/, float rotY/*=0*/, float rotZ/*=0*/ ) /*rotation to apply */
{
	// Set up matrices to control transformations and transform
	D3DXMATRIX WorldMat, TranslateMat, ScaleMat, RotateMat;
	D3DXMatrixTranslation(&TranslateMat, x, y, z);
	D3DXMatrixScaling(&ScaleMat,scaleX,scaleY,scaleZ);
	D3DXMatrixIdentity(&WorldMat);
	if(rotX!=0)
	{
		D3DXMatrixRotationX(&RotateMat, rotX);
		D3DXMatrixMultiply( &WorldMat, &WorldMat, &RotateMat);
	}
	if(rotY!=0)
	{
		D3DXMatrixRotationY(&RotateMat, rotY);
		D3DXMatrixMultiply( &WorldMat, &WorldMat, &RotateMat);
	}
	if(rotZ!=0)
	{
		D3DXMatrixRotationZ(&RotateMat, rotZ);
		D3DXMatrixMultiply( &WorldMat, &WorldMat, &RotateMat);
	}
	D3DXMatrixMultiply(&WorldMat, &WorldMat, &ScaleMat);
	D3DXMatrixMultiply( &WorldMat, &WorldMat, &TranslateMat);

	pD3D->SetTransform(D3DTS_WORLD, &WorldMat);
}