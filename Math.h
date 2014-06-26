#ifndef MATH_H
#define MATH_H

#include "Vector.h"

class Quaternion
{
public:
	Quaternion(void);
	Quaternion(float xp, float yp, float zp, float wp);
	~Quaternion(void);
	Quaternion QuaternionMultiply(Quaternion);
	Quaternion Inverse() ;
	Quaternion Normalize();
	Quaternion Scale(float s);
	Quaternion Conjugate();
	//Quaternion UnitQuaternion();
	float Norm();
	static float DotProduct(const Quaternion& q1, const Quaternion& q2);


	Vector3f QuaternionMultiply(Vector3f vec);
	Quaternion VectorToQuaternion(Vector3f vector);

	float x, y, z, w;

};

class Matrix4X4{

public:

	Matrix4X4()
	{
		for (int i = 0 ; i < 16; ++i)
		{
			if (i % 5 == 0)
				data[i] = 1;
			else
				data[i] = 0;
		}
	}
	void fromQuaternion(const Quaternion &q);
	void BuildRotationMatrix(const Vector3f& u, const float angle);
	void BuildTranslationMatrix(const Vector3f& v);
	Vector3f Normalize(const Vector3f &r);//returns unit vector
	float GetAngleRotation(const Vector3f &r);

	Vector3f MatrixMultiply(const Vector3f& v);

	Matrix4X4 MatrixMultiply(const Matrix4X4& other);

	union
	{
		struct
		{
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};
		float data[16];
	};
};
#endif