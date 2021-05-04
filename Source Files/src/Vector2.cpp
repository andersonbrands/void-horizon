#include <math.h>
#include <cassert>

#include "Vector2.h"
#include "defines.h"

using namespace UDEF;

namespace VEC
{



void Vec2::SetDegrees( float deg )
{
	x=cosf(D2R(deg));
	y=sinf(D2R(deg));
}

float Vec2::GetDegrees( void ) const
{
	float a=DotProduct(Vec2(1,0));
	if( a > 1.0f )
		a = 1.0f;
	else if( a < -1.0f )
		a = -1.0f;
	a=acosf(a);
	float d=R2D(a);	
	if(y<0)
		d=360-d;
	return d;
}

void Vec2::SetRadians( float rad )
{
	x=cosf(rad);
	y=sinf(rad);
}

float Vec2::Length() const
{
	return sqrtf( (x*x)+(y*y) );
}

void Vec2::Normalise()
{
	float rl = 1.0f / sqrtf( (x*x)+(y*y) );
	x *= rl;
	y *= rl;
}

void Vec2::RotateVector( float deg )
{
	float ca=cosf(D2R(deg));
	float sa=sinf(D2R(deg));
	float xn=(x*ca) - (y*sa);
	y=(y*ca) + (x*sa);
	x=xn;
}

bool Vec2::operator==( const Vec2 &rhs ) const
{
	//limited floating point accuracy means you won't get 0
	//so compare with a very small number instead
	if( (fabsf(this->x-rhs.x))>EPSILON )
		return false;
	if( (fabsf(this->y-rhs.y))>EPSILON )
		return false;
	return true;
}

bool Vec2::operator!=( const Vec2 &rhs ) const
{
	if( (fabsf(this->x-rhs.x))>EPSILON )
		return true;
	if( (fabsf(this->y-rhs.y))>EPSILON )
		return true;
	return false;
}
float MinDist(const Vec2 &e1, const Vec2 &e2, const Vec2 &p, Vec2 &pointOnLine)
{
	const float l2 = (e2-e1).SquaredLength();
	if(l2==0.f)//line segment is a point
	{
		pointOnLine=e1;
		return (p-e1).Length();
	}
	// Consider the line extending the segment, parameterized as e1 + t (e2 - e1).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-e1) . (e2-e1)] / |e2-e1|^2
	const float t = (p-e1).DotProduct(e2-e1) / l2;
	
	Vec2 dir(e2-e1);
	dir.Normalise();
	const float t2 = dir.DotProduct(p-e1)/sqrtf(l2);
	
	if (t < 0.0) 
	{
		pointOnLine=e1;
		return (p-e1).Length();       // Beyond the 'e1' end of the segment
	}
	else if (t > 1.0) 
	{
		pointOnLine=e2;
		return (p-e2).Length();  // Beyond the 'e2' end of the segment
	}
	//projection = e1 + t * (e2 - e1);  // Projection falls on the segment
	pointOnLine = (e2-e1);
	pointOnLine *= t;
	pointOnLine += e1;
	return (pointOnLine-p).Length();
}

bool CircleAABBCollision(const Vec2 &circPos, float circRadius, const Vec2 &boxCentre, float w, float h, Vec2 &collPos, Vec2 &collNorm)
{
	float left = boxCentre.x - (w/2 + circRadius);
	float right = boxCentre.x + (w/2 + circRadius);
	float top = boxCentre.y + (h/2 + circRadius);
	float bottom = boxCentre.y - (h/2 + circRadius);

	if(circPos.x<left || circPos.x>right || circPos.y>top || circPos.y<bottom)
		return false;

	float d = circPos.x-left;
	assert(d>=0);
	float closest=d;
	collPos=Vec2(left,circPos.y);
	collNorm=Vec2(-1,0);
	
	d = right-circPos.x;
	assert(d>=0);
	if(d<closest)
	{
		closest=d;
		collNorm=Vec2(1,0);
		collPos=Vec2(right,circPos.y);
	}
	d = top - circPos.y;
	assert(d>=0);
	if(d<closest)
	{
		closest=d;
		collNorm=Vec2(0,1);
		collPos=Vec2(circPos.x, top);
	}

	d = circPos.y-bottom;
	assert(d>=0);
	if(d<closest)
	{
		collNorm=Vec2(0,-1);
		collPos=Vec2(circPos.x, bottom);
	}
	return true;
}


bool CircleCircleCollision(const Vec2 &c1Pos, float c1Radius, const Vec2 &c2Pos, float c2Radius, Vec2 &collPos, Vec2 &collNorm)
{
	float dSq = (c1Pos-c2Pos).SquaredLength();
	float minDSq = (c1Radius+c2Radius)*(c1Radius+c2Radius);
	if(dSq>minDSq)
		return false;

	//point towards collision
	collNorm = c2Pos-c1Pos;
	collNorm.Normalise();
	
	//find where it happened
	collPos = c1Pos + collNorm * c1Radius;
	
	//point away from collision
	collNorm*=-1;
	return true;
}




}