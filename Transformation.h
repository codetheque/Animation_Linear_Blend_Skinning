#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <vector>
#include "Vector.h"
#include "Math.h"

class Transformation
{
public:
	Transformation(void);
	~Transformation(void);

	Vector3f rotation;
	Vector3f translation;
};

class WeightList
{
public:
	std::vector<float> weights;
};

class Frame
{
public:
	Frame(void) {}
	~Frame(void){}

	std::vector<Transformation> groups;
};

#endif