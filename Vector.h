#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#define MIN_NORM 0.0000001f

class Vector3f
{
public:

	Vector3f(float nx, float ny, float nz) :x(nx), y(ny), z(nz){}
	Vector3f() :x(0), y(0), z(0){}

	virtual ~Vector3f(void){}
	Vector3f Normalize();

	static float GetAngleRotation(const Vector3f &r)
	{
		return (float)sqrt((r.x*r.x)  + (r.y*r.y) + (r.z*r.z));
	}

	Vector3f operator+(Vector3f& other);
	void Set(Vector3f other);
	void Add(Vector3f other);
	void Subtract(Vector3f other);

	float x,y,z;
};
#endif