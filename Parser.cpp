#include "Parser.h"
#include "Transformation.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

Parser::Parser(void)
{
}

Parser::~Parser(void)
{
}

void Parser::ParseAnimationFile(const char* filename, std::vector<Frame> & frameVector)
{
	//Group of transformations.
	std::ifstream fin(filename);
	if (!fin)
	{
		std::cerr << "Failed to open " << filename << std::endl;
		return;
	}

	std::string line;
	while (getline(fin, line))
	{
		
		Frame frame;
        std::istringstream iss(line);
		
		while (iss)
   		{
			Transformation transform;

			iss >> transform.rotation.x;
			iss >> transform.rotation.y;
			iss >> transform.rotation.z;

			iss >> transform.translation.x;
			iss >> transform.translation.y;
			iss >> transform.translation.z;

			frame.groups.push_back(transform);
		}
		frameVector.push_back(frame);
	}
	fin.close();
}

void Parser::ParseWeightsFile(const char* filename, std::vector<WeightList>& weights)
{
	std::ifstream fin(filename);
	if (!fin)
	{
		std::cerr << "Failed to open " << filename << std::endl;
		return;
	}

	std::string line;
	while (getline(fin, line))
	{
		Frame frame;
        std::istringstream iss(line);
		WeightList list;
		while (iss)
   		{
			float val;
			iss >> val;

			list.weights.push_back(val);
		}
		weights.push_back(list);
	}
	fin.close();
}
