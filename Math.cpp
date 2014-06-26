/*
 References:
 Hanson, A.J., Visualizing Quaternions. The Morgan Kaufmann Series in Interactive 3D Technology. 
 Morgan Kaufmann Publishers Inc., San Francisco, CA, 2006.
 Dumnn, F., Parberry, I. 3D Math Primer for Graphics and Game Development. Worldware publishing Inc, 2002.
*/
#include "Math.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

#define M_PI	 3.1415926535f

Quaternion::Quaternion(void)
 : x(0), y(0), z(0), w(1) {}

Quaternion::Quaternion(float xp, float yp, float zp, float wp)
 : x(xp), y(yp), z(zp), w(wp)
{
}

Quaternion::~Quaternion(void){}

Quaternion Quaternion::QuaternionMultiply(Quaternion q)
{
	float xq = w * q.x + q.w * x + y * q.z - z * q.y;
	float yq = w * q.y + q.w * y + z * q.x - x * q.z;
	float zq = w * q.z + q.w * z + x * q.y - y * q.x;
	float wq = w * q.w - (x * q.x + y * q.y + z * q.z);

	return Quaternion(xq, yq, zq, wq);
}
Quaternion Quaternion::Normalize()
{
	Quaternion q(x,y,z,w);

	float magnitude = sqrt(q.Norm());

	if (magnitude > MIN_NORM)
	{
		q.x /= magnitude;
		q.y /= magnitude;
		q.z /= magnitude;
		q.w /= magnitude;
	}
	return q;
}

float Quaternion::Norm()
{
	float val = w*w + x*x + y*y + z*z;
	return val;
}

Quaternion Quaternion::Scale( float s)
{
	return Quaternion(x * s, y * s, z * s, w * s);
}

 Quaternion Quaternion::Inverse()
 {
	Quaternion q;
	q.x = x;
	q.y = y;
	q.z = z;
	q.w = w;

	q.Conjugate().Scale(1/q.Norm());
	return q;
 }

 Quaternion Quaternion::Conjugate()
 {
	 return Quaternion(-x, -y, -z, w);
 }

Vector3f Quaternion::QuaternionMultiply(Vector3f vec)
 {	 
	 Quaternion vectorQuaternion = VectorToQuaternion(vec);
	 Quaternion inverse = Inverse();
	 Quaternion result = vectorQuaternion.QuaternionMultiply(inverse);
	 result.Normalize();
	 Vector3f  r;
	 r.x = result.x;
	 r.y = result.y;
	 r.z = result.z;
	 return r;
}

Quaternion Quaternion::VectorToQuaternion(Vector3f vector)
 {
	Quaternion q;
	q.x = vector.x;
	q.y = vector.y;
	q.z = vector.z;
	q.w = 0.0f;

	return q;
}

float  Quaternion::DotProduct(const Quaternion& q1, const Quaternion& q2)
{ 
	return ( q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
}

//Setup the matrix to perform a rotation, given the angular displacement
//in quaternion form.
void Matrix4X4::fromQuaternion(const Quaternion &q)
{
	float xx = 2.0f * q.x;
	float yy = 2.0f * q.y;
	float zz = 2.0f * q.z;
	float ww = 2.0f * q.w;

	m11 = 1.0f - yy * q.y - zz * q.z;
	m12 = xx * q.y + ww * q.z;
	m13 = xx * q.z - ww * q.x;

	m21 = xx * q.z - ww * q.z;
	m22 = 1.0f - xx * q.x - zz * q.z;
	m23 = yy * q.z + ww * q.x;

	m31 = xx * q.z + ww * q.y;
	m32 = yy * q.z - ww * q.x;
	m33 = 1.0f - xx * q.x - yy * q.y;

	//Reset the translation
	m41 = m42 = m43 = 0.0f;
	m44 = 1;

}

Vector3f Matrix4X4::MatrixMultiply(const Vector3f& v)
{
	float x = m11*v.x + m21*v.y + m31*v.z;
	float y = m12*v.x + m22*v.y + m32*v.z;
	float z = m13*v.x + m23*v.y + m33*v.z;

	return Vector3f(x, y, z);
}

//Rotation Matrix from axis angle (Popa,T. Slides - Lecture 2)
void Matrix4X4::BuildRotationMatrix(const Vector3f &u, float alpha)
{
	float ux = u.x * u.x;
	float uy = u.y * u.y;
	float uz = u.z * u.z;

	assert(fabs((ux+uy+uz) - 1.0f) < 1.0f);

	float sine = sin(alpha);
	float cosine = cos(alpha);

	m11 = ux + cosine * (1 - ux);
	m21 = u.x * u.y * (1 - cosine) - u.z * sine;
	m31 = u.x * u.z * (1 - cosine) + u.y * sine;
	m41 = 0;

	m12 = u.x * u.y * (1 - cosine) + u.z * sine;
	m22 = uy + cosine * (1 - uy);
	m32 = u.y * u.z * (1 - cosine) - u.x * sine;
	m42 = 0;

	m13 = u.x * u.z * (1 - cosine) - u.y * sine;
	m23 = u.y * u.z * (1 - cosine) + u.x * sine;
	m33 = uz + cosine * (1 - uz);
	m43 = 0;

	m41 = 0;
	m42 = 0;
	m43 = 0;
	m44 = 1;

}

void Matrix4X4::BuildTranslationMatrix(const Vector3f& v)
{
	m11 = 1; m12 = m13 = 0; m14 = 0;
	m21 = 0; m22 = 1; m23 = 0; m24 = 0;
	m31 = m32 = 0; m33 = 1; m34 = 0;
	m41 = v.x;
	m42 = v.y;
	m43 = v.z; 
	m44 = 1;
}

Matrix4X4 Matrix4X4::MatrixMultiply(const Matrix4X4& other)
{
	Matrix4X4 result;

	result.m11 = m11 * other.m11   +   m12 * other.m21   +   m13 * other.m31   +   m14 * other.m41;    
	result.m12 = m11 * other.m12   +   m12 * other.m22   +   m13 * other.m32   +   m14 * other.m42;    
	result.m13 = m11 * other.m13   +   m12 * other.m23   +   m13 * other.m33   +   m14 * other.m43;    
	result.m14 = m11 * other.m14   +   m12 * other.m24   +   m13 * other.m34   +   m14 * other.m44;    
		  
	result.m21 = m21 * other.m11   +   m22 * other.m21   +   m23 * other.m31   +   m24 * other.m41;   
	result.m22 = m21 * other.m12   +   m22 * other.m22   +   m23 * other.m32   +   m24 * other.m42;    
	result.m23 = m21 * other.m13   +   m22 * other.m23   +   m23 * other.m33   +   m24 * other.m43;   
	result.m24 = m21 * other.m14   +   m22 * other.m24   +   m23 * other.m34   +   m24 * other.m44;    
		   
 
	result.m31 = m31 * other.m11   +   m32 * other.m21   +   m33 * other.m31   +   m34 * other.m41;   
	result.m32 = m31 * other.m12   +   m32 * other.m22   +   m33 * other.m32   +   m34 * other.m42;    
	result.m33 = m31 * other.m13   +   m32 * other.m23   +   m33 * other.m33   +   m34 * other.m43;    
	result.m34 = m31 * other.m14   +   m32 * other.m24   +   m33 * other.m34   +   m34 * other.m44;    
	
	result.m41 = m41 * other.m11   +   m42 * other.m21   +   m43 * other.m31   +   m44 * other.m41;    
	result.m42 = m41 * other.m12   +   m42 * other.m22   +   m43 * other.m32   +   m44 * other.m42;    
	result.m43 = m41 * other.m13   +   m42 * other.m23   +   m43 * other.m33   +   m44 * other.m43;    
	result.m44 = m41 * other.m14   +   m42 * other.m24   +   m43 * other.m34   +   m44 * other.m44;    

	return result;
}
