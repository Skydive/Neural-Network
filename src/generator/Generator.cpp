#include "Generator.hpp"

extern "C" Generator* create_object()
{
	return new Generator();
}

extern "C" void destroy_object(Generator* object)
{
	delete object;
}

std::vector<TrainingData>* Generator::GenerateTrainingData(){}
