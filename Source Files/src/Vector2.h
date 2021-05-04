#ifndef VECTOR2_H
#define VECTOR2_H



namespace VEC
{


/*
Handy class for manipulating 2d vectors
*/
class Vec2
{
public:
	float x, y;


	Vec2() : x(0), y(0) {}
	Vec2( const Vec2 &rhs ) : x(rhs.x), y(rhs.y) {}
	Vec2( float x_, float y_ ) : x(x_), y(y_) {}
	//convenience operator overloads
	void operator=(const Vec2 &rhs)
	{
		this->x=rhs.x;
		this->y=rhs.y;
		//return *this;
	}
	bool operator==(const Vec2 &rhs) const;
	bool operator!=(const Vec2 &rhs)const;
	Vec2 operator+(const Vec2 &rhs) const
	{
		Vec2 res;
		res.x=this->x+rhs.x;
		res.y=this->y+rhs.y;
		return res;
	}
	Vec2 operator-() const
	{
		Vec2 res;
		res.x=-this->x;
		res.y=-this->y;
		return res;
	}
	Vec2 operator-(const Vec2 &rhs) const
	{
		Vec2 res;
		res.x=this->x-rhs.x;
		res.y=this->y-rhs.y;
		return res;
	}
	void operator+=(const Vec2 &rhs) 
	{
		this->x+=rhs.x;
		this->y+=rhs.y;
	}
	void operator-=(const Vec2 &rhs)
	{
		this->x-=rhs.x;
		this->y-=rhs.y;
	}
	void operator/=(const Vec2 &rhs)
	{
		this->x/=rhs.x;
		this->y/=rhs.y;
	}
	void operator/=(const float a)
	{
		this->x/=a;
		this->y/=a;
	}
	Vec2 operator*(const float a) const
	{
		Vec2 res;
		res.x=this->x*a;
		res.y=this->y*a;
		return res;
	}
	Vec2 operator*(const Vec2 &a) const 
	{
		Vec2 res;
		res.x=this->x*a.x;
		res.y=this->y*a.y;
		return res;
	}
	void operator*=(const float a)
	{
		this->x*=a;
		this->y*=a;
	}

	//set a rotation on a unit vector (a direction)
	//setDegrees(0) == Vec2(1,0)
	//setDegrees(90) == Vec2(0,1)
	//setDegrees(180) == Vec2(-1,0)
	void SetDegrees(float deg);
	float GetDegrees(void) const;
	//alternate if you like using radians
	void SetRadians(float rad);

	void SetValue(float x_, float y_)
	{
		x=x_;
		y=y_;
	}

	//useful functions
	float Length() const;
	//for fast comparisons of length
	float SquaredLength() const
	{
		return (x*x)+(y*y);
	}
	//turn it into a normal or direction
	void Normalise();
	//dot products are very handy
	float DotProduct( const Vec2 &rhs ) const
	{
		return (x*rhs.x)+(y*rhs.y);
	}
	//take an existing vector and rotate it
	//assuming it's a direction (1 long)
	void RotateVector(float deg);
};

/*
Closest distance of a point to a line
e1 - one end of a line
e2 - the other end of a line
p  - a point in space
pointOnLine - the closest position on the line
returns - closest distance to the line
*/
float MinDist(const Vec2 &e1, const Vec2 &e2, const Vec2 &p, Vec2 &pointOnLine);

/*
Test if a circle and a rectangle are colliding
AABB - axis aligned bounding box
circPos - centre of circle
circRadius - circle radius
boxCentre - centre of rectangle
w/h - width and height of rectangle
collPos - collision point
collNorm - normal of collision
returns - true if the two are colliding
*/
bool CircleAABBCollision(const Vec2 &circPos, float circRadius, const Vec2 &boxCentre, float w, float h, Vec2 &collPos, Vec2 &collNorm);

/*
Test if two circles are colliding.
c1Pos/c1Radius - the subject of the testing
c2Pos/c2Radius - a potential collider
collPos - point of contact
collNorm - escape direction for circle1
*/
bool CircleCircleCollision(const Vec2 &c1Pos, float c1Radius, const Vec2 &c2Pos, float c2Radius, Vec2 &collPos, Vec2 &collNorm);



}




#endif