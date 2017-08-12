
#include "Network.hpp"
//#include "ImageWindow.hpp"
//#include <mnist/mnist_reader.hpp>

#include <iostream>
#include <string>

#include <bitset>

#include <Eigen/Core>
using namespace Eigen;

double Cost(Network& N, std::vector<TrainingData>& T)
{
	double Sum = 0.0;
	for(const TrainingData& t : T)
	{
		N.SetInput(t.Inputs, nullptr);
		N.FeedForward();
		VectorXd Outputs = N.GetOutput();
		Sum += 0.5*((t.DesiredOutputs-Outputs).squaredNorm());
	}
	return Sum/T.size();
}

int main(int argc, char** argv)
{
	// Create Network
	std::cout << "Creating Network:" << std::endl;
	Network NeuralNetwork;
	NeuralNetwork.AddLayer(16, NEURON_GAUSSIAN);
	NeuralNetwork.AddLayer(10, NEURON_GAUSSIAN);
	NeuralNetwork.AddLayer(8, NEURON_GAUSSIAN);

	for(int i=0; i<NeuralNetwork.Layers.size(); i++)
	{
		std::cout << "\tLayer:\t" << i << std::endl
		 	 << "\t\tNodes:\t" << NeuralNetwork.Layers[i].Outputs.size() << std::endl;
	}
	std::cout << std::endl;

	// Generate Training Data
	std::cout << "Generating Training Data:" << std::endl;
	std::vector<TrainingData> T;
	int limit = pow(2, 16);
	for(int i=0; i<limit; i++)
	{
		TrainingData t;
		t.DesiredOutputs = VectorXd::Zero(8);
		t.Inputs = VectorXd::Zero(16);

		std::string in = std::bitset<16>(i).to_string();
		int in2 = i; int in1 = i >> 8;
		std::string out = std::bitset<8>(in1 | in2).to_string();
		for(int k=0; k<t.Inputs.size(); k++)
			t.Inputs(k) = ((int)(in[k]) == (int)("1")[0]) ? 1.0 : 0.0;
		for(int k=0; k<t.DesiredOutputs.size(); k++)
			t.DesiredOutputs(k) = ((int)(out[k]) == (int)("1")[0]) ? 1.0 : 0.0;
		T.push_back(t);
	}
	std::cout << "\tCost:\t\t" << Cost(NeuralNetwork, T) << std::endl
		 << "\tInput Size:\t" << T.size() << std::endl << std::endl;


	// Perform Stochastic Gradient Descent - Train the network
	double eta = 30.0;
	int batch = 10;
	int epochs = 10;
	std::cout << "Training Network:" << std::endl
		 << "\tLearning Rate:\t" << eta << std::endl
		 << "\tBatch Size:\t" << batch << std::endl
		 << "\tEpochs:\t\t" << epochs << std::endl << std::endl;

	NeuralNetwork.SGD(T, epochs, batch, eta);

	// Integrity check
	std::cout << "Network Integrity Test: " << std::endl;
	int correct = 0;
	for(const TrainingData& t : T)
	{
		NeuralNetwork.SetInput(t.Inputs, nullptr);
		NeuralNetwork.FeedForward();
		VectorXd SanitizedOutputs = NeuralNetwork.GetOutput().unaryExpr([](double x)
		{
			return (x > 0.9) ? 1 : (
				   (x < 0.1) ? 0 :
				   0.5);
		});
		correct += (SanitizedOutputs == t.DesiredOutputs) ? 1 : 0;
	}

	std::cout << "\tRatio:\t\t" << correct << "/" << T.size() << std::endl
	 	 << "\tCost:\t\t" << Cost(NeuralNetwork, T) << std::endl << std::endl;


	const auto GetInput = [](std::string prompt, std::function<bool(std::string)> ValidFunc) {
		bool bValid=false;
		while(!bValid)
		{
			std::cout << prompt;
			std::string response;
			std::cin >> response;
			if(ValidFunc == nullptr)
				return response;
			if(ValidFunc(response))
				return response;
			std::cout << "Your input is invalid, please enter a valid input" << std::endl;
		}
	};

	const auto VALID_8BIT_BINARY = [](std::string in){
		for(int i=0; i<in.size(); i++)
		{
			if(((int)(in[i]) != (int)("1")[0])
			&& ((int)(in[i]) != (int)("0")[0]))
			{
				return false;
			}
		}
		return in.size() == 8;
	};

	while(true)
	{
		std::string Input1 = GetInput("Enter first binary number: ", VALID_8BIT_BINARY);
		std::string Input2 = GetInput("Enter second binary number: ", VALID_8BIT_BINARY);
		std::string in = Input1 + Input2;
		VectorXd Input = VectorXd::Zero(16);
		for(int k=0; k<Input.size(); k++)
			Input(k) = ((int)(in[k]) == (int)("1")[0]) ? 1.0 : 0.0;
		NeuralNetwork.SetInput(Input, nullptr);
		NeuralNetwork.FeedForward();
		VectorXd SanitizedOutputs = NeuralNetwork.GetOutput().unaryExpr([](double x)
		{
			return (x > 0.9) ? 1 : (
				   (x < 0.1) ? 0 :
				   0.5);
		});
		std::string result;
		for(int i=0; i<SanitizedOutputs.size(); i++)
		{
			result += std::to_string((int)SanitizedOutputs(i));
		}
		std::cout << "Result: " << result << std::endl << std::endl;
	}
	/*

	if(argc == 2)
	{
		if(argv[1] == "train")
			Train(Network);

		if(argv[1] == "cost")
			Cost(Network);
	}*/

	/*
	Network NeuralNetwork;
	NeuralNetwork.AddLayer(28*28, NEURON_GAUSSIAN);
	NeuralNetwork.AddLayer(30, NEURON_GAUSSIAN);
	NeuralNetwork.AddLayer(10, NEURON_GAUSSIAN);

	auto dataset = mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>("./data");
	std::cout << "Nbr of training images = " << dataset.training_images.size() << std::endl;
    std::cout << "Nbr of training labels = " << dataset.training_labels.size() << std::endl;
    std::cout << "Nbr of test images = " << dataset.test_images.size() << std::endl;
    std::cout << "Nbr of test labels = " << dataset.test_labels.size() << std::endl;

	ImageWindow IW(dataset.training_images, 28, 28, 10);
	thread t = IW.Start();

	std::cout << "Training" << std::endl;

	NeuralNetwork.SGD(dataset.training_labels, dataset.training_images, 1, 10, 3.0);

	double Cost = 0.0;
	int i;
	for(i=0; i<dataset.training_images.size(); i++)
	{
		IW.ImageIndex = i;

		// Perform learning for this particular image here
		//std::this_thread::sleep_for(std::chrono::milliseconds(25));

		vector<double> Output;
		NeuralNetwork.ProvideInput<uint8_t>(dataset.training_images[i], [](uint8_t c){ return c / 255.0; });
		NeuralNetwork.FeedForward(&Output);
		if(i % 10000 == 0)
		{
			std::cout << "Image: " << i << std::endl;
			std::cout << "Label: " << (int)dataset.training_labels[i] << std::endl;
			NeuralNetwork.PrintOutput();
			std::cout << std::endl;
		}

		int num = dataset.training_labels[i];
		vector<double> DesiredOutput;
		for(int j=0; j<10; j++)
		{
			DesiredOutput.push_back((j == num) ? j : 0);
		}
		vector<double> diffsq;
		transform(Output.begin(), Output.end(), DesiredOutput.begin(), back_inserter(diffsq), [](double l, double r){
			return pow(l - r, 2);
		});
		Cost += accumulate(diffsq.begin(), diffsq.end(), 0, [](double l, double r){
			return l+r;
		});

		if(IW.bDead)
			break;
	}
	Cost /= (2*(i+1));
	std::cout << "Cost: " << Cost << std::endl;
	std::cout << "Image Index: " << IW.ImageIndex << std::endl;
	IW.bDead = true;
	t.join();
	*/
}
