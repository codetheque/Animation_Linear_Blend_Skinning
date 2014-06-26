#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Transformation.h"

class Parser
{
public:
	Parser(void);
	~Parser(void);

	void ParseAnimationFile(const char* filename, std::vector<Frame>& frameVector); 
	void ParseWeightsFile(const char* filename, std::vector<WeightList>& weights); 
};
#endif