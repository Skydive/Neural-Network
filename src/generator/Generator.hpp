#pragma once
#include "../Network.hpp"

class Generator
{
public:
	virtual std::vector<TrainingData>* GenerateTrainingData();
	std::vector<TrainingData> T;
};
