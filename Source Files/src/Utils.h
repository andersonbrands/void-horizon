#ifndef UTILS_H
#define UTILS_H

/*
Functions and data we think are generic and are sure will be useful in other simple games. 
To keep this clean, as more functions are added, this would need to be split into
multiple files with sensible names to keep related functions/data together.
*/
#include <climits>
#include <d3dx9.h>


// Call once at start to seed random number generator.
// If no seed given it uses current time.
void SeedRandom(unsigned int seed=UINT_MAX);
// Get a random number in the range min->max
int GetRandom(int min, int max);
float GetRandom(float min, float max);

//setup a new world transform
void SetTransform(	LPDIRECT3DDEVICE9 pD3D,
					float x, float y, float z, /*position */ 
					float scaleX=1.f, float scaleY=1.f, float scaleZ=1.f,  /*scale to apply */
					float rotX=0, float rotY=0, float rotZ=0 ); /*rotation to apply */
#endif
