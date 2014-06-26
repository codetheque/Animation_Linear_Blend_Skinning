#include "Vector.h"

// rx,ry,rz 
Vector3f Vector3f::Normalize()
{
	Vector3f u;//Unit vector
	float magnitude = sqrt(x*x  + y*y + z*z);
	
	if (magnitude > MIN_NORM)
	{
		u.x = x/magnitude;
		u.y = y/magnitude;
		u.z = z/magnitude;
	}

	return u;
}
Vector3f Vector3f::operator+(Vector3f& other)
{
	Vector3f v(x,y,z);
	v.x = other.x + x;
	v.y = other.y + y;
	v.z = other.z + z;

	return v;
}

void Vector3f::Set(Vector3f other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vector3f::Add(Vector3f other)
{
	x = other.x + x;
	y = other.y + y;
	z = other.z + z;
}

void Vector3f::Subtract(Vector3f other)
{
	x =  x - other.x;
	y =  y - other.y;
	z =  z - other.z;
}