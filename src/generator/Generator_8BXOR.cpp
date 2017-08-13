#include "Generator.hpp"
#include <string>
#include <bitset>

#include <Eigen/Core>
using namespace Eigen;
extern "C" Generator* create_object()
{
	return new Generator();
}

extern "C" void destroy_object(Generator* object)
{
	delete object;
}

std::vector<TrainingData>* Generator::GenerateTrainingData()
{
	T.clear();
	std::cout << "\tGenerator_8BXOR.so executed.." << std::endl;
	int limit = pow(2, 16);
	for(int i=0; i<limit; i++)
	{
		TrainingData t;
		t.DesiredOutputs = VectorXd::Zero(8);
		t.Inputs = VectorXd::Zero(16);

		std::string in = std::bitset<16>(i).to_string();
		int in2 = i; int in1 = i >> 8;
		std::string out = std::bitset<8>(in1 ^ in2).to_string();
		for(int k=0; k<t.Inputs.size(); k++)
			t.Inputs(k) = ((int)(in[k]) == (int)("1")[0]) ? 1.0 : 0.0;
		for(int k=0; k<t.DesiredOutputs.size(); k++)
			t.DesiredOutputs(k) = ((int)(out[k]) == (int)("1")[0]) ? 1.0 : 0.0;
		T.push_back(t);
	}
	return &T;
}
