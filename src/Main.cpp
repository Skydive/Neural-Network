
#include "Network.hpp"
//#include "ImageWindow.hpp"

#include <iostream>
#include <string>
#include <bitset>
#include <map>

#include <Eigen/Core>
using namespace Eigen;


#include <sstream>
#include <fstream>
namespace File
{
	void Write(const std::string& path, const std::string& data)
	{
		std::ofstream file(path.c_str(), std::ios::trunc);
		file << data;
		file.close();
	}
	std::string Read(const std::string& path)
	{
		std::ifstream file(path);
		std::string content((std::istreambuf_iterator<char>(file)),
        					(std::istreambuf_iterator<char>()));
		return content;
	}
}

void Create(Network& N, const std::string& instr)
{
	N.Layers.clear();
	std::cout << "Creating Network:" << std::endl;

	std::istringstream iss(instr);
	std::string s;
	while(std::getline(iss, s, ' '))
	{
		N.AddLayer(std::stoi(s), NEURON_GAUSSIAN);
	}

	for(int i=0; i<N.Layers.size(); i++)
	{
		std::cout << "\tLayer:\t" << i << std::endl
		 	 	  << "\t\tNodes:\t" << N.Layers[i].Outputs.size() << std::endl
				  << "\t\tWeight:\t" << N.Layers[i].Weights.size() << std::endl;
	}
	std::cout << std::endl;
}

void Train(Network& NeuralNetwork, const std::string& instr)
{
	std::cout << "Acquiring Training Data:" << std::endl;
	std::cout << "\tInput Size:\t" << NeuralNetwork.T->size() << std::endl << std::endl;
	std::cout << "Network Cost:\t\t" << NeuralNetwork.Cost() << std::endl << std::endl;

	// Default parameters
	std::vector<double> in = {10.0, 10.0, 3.0};
	if(instr != "default")
	{
		in.clear();
		std::istringstream iss(instr);
		std::string s;
		while(std::getline(iss, s, ' '))
		{
			in.push_back(std::stod(s));
		}
	}
	int epochs = in[0], batch = in[1];
	double eta = in[2];
	std::cout << "Training Network:" << std::endl
			  << "\tLearning Rate:\t" << eta << std::endl
			  << "\tBatch Size:\t" << batch << std::endl
			  << "\tEpochs:\t\t" << epochs << std::endl << std::endl;
	NeuralNetwork.SGD(epochs, batch, eta);

	std::cout << "Training Completed" << std::endl << std::endl;
}

void Write(Network& N, const std::string& path)
{
	std::cout << "Writing to: " << path << std::endl << std::endl;
	std::string str; N.Serialize(str);
	File::Write(path, str);
}

void Read(Network& N, const std::string& path)
{
	std::cout << "Reading from: " << path << std::endl << std::endl;
	std::cout << "Regenerating Network:" << std::endl;

	std::string str = File::Read(path);
	N.Deserialize(str);

	for(int i=0; i<N.Layers.size(); i++)
	{
		std::cout << "\tLayer:\t" << i << std::endl
				  << "\t\tNodes:\t" << N.Layers[i].Outputs.size() << std::endl
				  << "\t\tWeight:\t" << N.Layers[i].Weights.size() << std::endl;
	}
	std::cout << std::endl;
}

void IntegrityTest(Network& NeuralNetwork)
{
	std::cout << "Network Integrity Test: " << std::endl;
	int correct = 0;
	for(const TrainingData& t : *NeuralNetwork.T)
	{
		NeuralNetwork.SetInput(t.Inputs, nullptr);
		NeuralNetwork.FeedForward();
		VectorXd SanitizedOutputs = NeuralNetwork.GetOutput().unaryExpr(SANITIZE_OUTPUT);
		correct += (SanitizedOutputs == t.DesiredOutputs) ? 1 : 0;
	}
	std::cout << "\tRatio:\t\t" << correct << "/" << NeuralNetwork.T->size() << std::endl
			  << "\tCost:\t\t" << NeuralNetwork.Cost() << std::endl << std::endl;
}

void FeedForward(Network& NeuralNetwork, const std::string& instr)
{
	std::istringstream iss(instr);
	std::string s;
	VectorXd Input;
	while(std::getline(iss, s, ' '))
	{
		Input.conservativeResize(Input.size() + 1, NoChange);
		Input(Input.size()-1) = std::stod(s);
	}
	NeuralNetwork.SetInput(Input, nullptr);
	NeuralNetwork.FeedForward();
}

void Cycle(int& i, int arg)
{
	static int cycle = 0;
	i = arg;
	cycle++;
	std::cout << "Cycle: " << cycle << std::endl << std::endl;
}

using CommandMap = std::map<std::string, std::function<void(Network&, int&, char**)>>;
CommandMap COMMANDLET_LIST = {
	{"-create",		[](Network& N, int& i, char** argv) { Create(N, std::string(argv[i+1])); }},
	{"-train",		[](Network& N, int& i, char** argv) { Train(N, std::string(argv[i+1])); }},
	{"-write",		[](Network& N, int& i, char** argv) { Write(N, std::string(argv[i+1])); }},
	{"-read",		[](Network& N, int& i, char** argv) { Read(N, std::string(argv[i+1])); }},
	{"-test",		[](Network& N, int& i, char** argv) { IntegrityTest(N); }},
	{"-feedforward",[](Network& N, int& i, char** argv) { FeedForward(N, std::string(argv[i+1])); }},
	{"-output",		[](Network& N, int& i, char** argv) { N.PrintOutput(false); }},
	{"-outputfancy",[](Network& N, int& i, char** argv) { N.PrintOutput(true); }},
	{"-generate",	[](Network& N, int& i, char** argv) { N.GenerateTrainingData(std::string(argv[i+1])); }},
	{"-cycle",		[](Network& N, int& i, char** argv) { Cycle(i, std::stoi(std::string(argv[i+1]))); }}
};

int main(int argc, char** argv)
{
	Network NeuralNetwork;

	for(int i=1; i<argc; i++)
		if(COMMANDLET_LIST.find(std::string(argv[i])) != COMMANDLET_LIST.end())
			COMMANDLET_LIST[std::string(argv[i])](NeuralNetwork, i, argv);
}
