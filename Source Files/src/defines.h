#ifndef DEFINES_H
#define DEFINES_H

#include <string>
#include <d3dx9math.h>		

/*
Magic numbers unlikely to change, kept all in once place
*/
namespace UDEF
{

const char LINE_FEED = 10; ///<an ascii line feed character code
const char CR_RETURN = 13; ///<carriage return ascii code
const unsigned short UNICODE_HEADER = 0xfeff;
const unsigned int MAX_UINT = 0xffffffff;
const int MAX_INT   = 0x7fffffff;
const float MAX_REAL = 3.40282e+038f;
const unsigned short MAX_USHORT = 0xffff;
const unsigned char MAX_UCHAR = 0xff;
// used when we want to allow a very tiny tolerance around zero
const float EPSILON = 10e-9f;
// used to allow a larger tolerance to floating point comparisons
const float BIG_EPSILON = 10e-7f;
// bigger tolerance again...
const float EVEN_BIGGER_EPSILON = 10e-4f;
// bigger again
const float EVEN_BIGGER_AGAIN_EPSILON = 10e-2f;
// an even bigger tolerance value
const float HUGE_EPSILON = 1.0f;
//a general purpose text buffer size big enough for the biggest path or mssg
const int MAX_TXT = 2048;		

extern float g_PI;
extern float g_2PI;
extern char g_txt[MAX_TXT];				//these buffers are not safe between functions!!!
//convert to and from degrees and radians
inline float D2R(float x) { return (x * g_PI)/180.0f; }
inline float R2D(float x) { return (x * 180.0f) / g_PI; }

const float BORDER_LEFT = -45;
const float BORDER_TOP = 75;
const float BORDER_RIGHT = 105;
const float BORDER_DOWN = -5;// should have been defined as BORDER_BOTTOM for "consistency" matters, will keep in order to avoid conflicts
const float BORDER_BOTTOM = BORDER_BOTTOM;// making BORDER_BOTTOM available

}

#endif