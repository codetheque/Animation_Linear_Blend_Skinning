#ifndef ANIMATION_H
#define ANIMATION_H

#include "Vector.h"
#include <vector>
#include "Transformation.h"
#include "Math.h"
#include "glm.h"
#include <assert.h>

struct Bone
{
	Bone() : parent(0), id(0) {}

	int id;
	std::vector<Bone*> child;
	Vector3f firstJoint;
	Vector3f secondJoint;
	Bone* parent;
};

struct Skeleton
{
	~Skeleton()
	{
		for (unsigned int i = 0; i < bones.size(); ++i)
		{
			delete bones[i];
		}
		bones.clear();
	}

	//Build the bone tree.
	void AddBone(Bone* bone, int parentId)
	{
		if (root != nullptr && parentId != -1)
		{
			if (AddBoneRecursive(root, bone, parentId))
			{
				bones.push_back(bone);
			}
			else
			{
				assert(false);
			}
		}
		else if (parentId == -1)
		{
			root = bone;
			bones.push_back(bone);
		}
	}

	//Recursively search the parent of a given bone.
	bool AddBoneRecursive(Bone* parent, Bone* bone, int parentId)
	{
		if (parent->id == parentId)
		{
			parent->child.push_back(bone);
			bone->parent = parent;
			return true;
		}
		else
		{
			for (unsigned int i = 0; i < parent->child.size(); ++i)
			{
				if (AddBoneRecursive(parent->child[i], bone, parentId))
				{
					return true;
				}
			}
		}

		return false;
	}
	std::vector<Bone*> bones;

	Bone* root;
};

class Animation
{
public:
	Animation(void) ;
	virtual ~Animation(void);

	void Update(float elapsedTime);
	void Draw(GLMmodel* model);

	std::vector<Frame> animationKeyframesVector;
	Skeleton skeleton;

	//Debug.
	void IncMaxBonesDraw();
	void DecMaxBonesDraw();
	void ToggleMatrix() { useMatrix = !useMatrix;}
	//End debug.

private:
	Vector3f UpdateBone(Transformation boneTransform, Vector3f bonePos);
	void DrawRoot();
	void DrawBone(Bone* bone, Vector3f startingPoint);
	void DrawModel(GLMmodel* model);

	unsigned int currentFrame;
	float frame;
	//Debug.
	bool useMatrix;
};
#endif