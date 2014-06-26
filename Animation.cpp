#include "Animation.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

using std::ifstream;

std::ofstream logFile("interpolation.log");
bool useLog = false;

GLUquadricObj *quadratic;

#define USE_MATRIX 1

#define DRAW_FRAME_BY_FRAME 1
int maxBones = 17;
int numDrawBones = 0;

Animation::Animation(void) : currentFrame(2), frame(0), useMatrix(true)
{
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
}

Animation::~Animation(void) 
{
	gluDeleteQuadric(quadratic);
}

void Animation::Update(float elapsedTime)
{
	//Play the animation
	if (!animationKeyframesVector.empty())
	{
		frame += elapsedTime;

		currentFrame++;
		if (currentFrame > animationKeyframesVector.size() - 1)
		{
			currentFrame = 0;
		}
	
		//Log the frame time
		if(useLog)
		{
			logFile << "frame " << currentFrame << " elapsedTime " << elapsedTime  << std::endl;
		}
	}
}

void Animation::IncMaxBonesDraw()
{
	maxBones++;
}

void Animation::DecMaxBonesDraw()
{
	maxBones--;
	if (maxBones < 1)
		maxBones = 1;
}

/*
 Calculate the bone position based on the transformation.
 Transformation comes from the animationKeyframesVector ('simple walking' dataset).
*/
Vector3f Animation::UpdateBone(Transformation boneTransform, Vector3f bonePos)
{
	Vector3f v = boneTransform.rotation.Normalize();
	
	float angle = Vector3f::GetAngleRotation(v);

	Matrix4X4 m;
	m.BuildRotationMatrix(v, angle);

	Vector3f secondJoint = m.MatrixMultiply(bonePos);

	return secondJoint;
}

//Draw the root bone
void Animation::DrawRoot()
{
	if (skeleton.root == nullptr)
		return;

	Frame frame = animationKeyframesVector[currentFrame];
	Transformation boneTransform = frame.groups[skeleton.root->id];

	Vector3f firstJoint;
	Vector3f secondJoint;
	if (useMatrix)
	{
		secondJoint = UpdateBone(boneTransform, skeleton.root->secondJoint);
		firstJoint = secondJoint;
	}
	else
	{
		firstJoint.Set(skeleton.root->firstJoint);
		secondJoint.Set(skeleton.root->secondJoint);
	}

	//Draw just one sphere to represent the root bone.
	glPushMatrix();
	{
		glTranslatef(secondJoint.x, secondJoint.y, secondJoint.z);
		glColor3f(1,1,1);
		gluSphere(quadratic,0.015,10,10);
	}
	glPopMatrix();
	
	//Keep the parent matrix in the stack.
	for (unsigned int i = 0; i < skeleton.root->child.size(); ++i)
	{
		DrawBone(skeleton.root->child[i], secondJoint);
	}
}

//Draw any bone from the starting point.
void Animation::DrawBone(Bone* bone, Vector3f startingPoint)
{
#if DRAW_FRAME_BY_FRAME
	if(numDrawBones > maxBones)
	{
		return;
	}
	numDrawBones++;
#endif

	Frame frame = animationKeyframesVector[currentFrame];
	Transformation boneTransform = frame.groups[bone->id];
	
	Vector3f secondJoint;
	if (useMatrix)
	{
		secondJoint = UpdateBone(boneTransform, bone->secondJoint);
	}
	else
	{
		secondJoint.Set(bone->secondJoint);
	}

	glPushMatrix();
	{
		//Draw the fist joint.
		glPushMatrix();
		{
			glTranslatef(startingPoint.x, startingPoint.y, startingPoint.z);
			glColor3f(1,0,0);
			gluSphere(quadratic,0.01,10,10);
		}
		glPopMatrix();

		//Draw the second joint.
		glPushMatrix();
		{
			glTranslatef(secondJoint.x, secondJoint.y, secondJoint.z);		
			glColor3f(0,1,0);
			gluSphere(quadratic,0.01,10,10);
		}
		glPopMatrix();

		//Draw the bone linking both joints.
		glPushMatrix();
		{
			glColor3f(1,1,1);
			glBegin(GL_LINES);
				glVertex3f(startingPoint.x, startingPoint.y, startingPoint.z);
				glVertex3f(secondJoint.x, secondJoint.y, secondJoint.z);
			glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	//Keep the parent matrix in the stack.
	for (unsigned int i = 0; i < bone->child.size(); ++i)
	{
		DrawBone(bone->child[i], secondJoint);
	}
}

void Animation::Draw(GLMmodel* model)
{
#if DRAW_FRAME_BY_FRAME
	numDrawBones = 0;
#endif

	DrawRoot();
	DrawModel(model);
}

//Skin it
void Animation::DrawModel(GLMmodel* model)
{
		//Multiply by each part.
}
