#include "Generator.hpp"
#include <string>
#include <bitset>

#include <Eigen/Core>
using namespace Eigen;

#include <mnist/mnist_reader.hpp>

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
	std::cout << "\tGenerator_MNIST.so executed.." << std::endl;
	auto dataset = mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>("./data");
	/*std::cout << "Nbr of training images = " << dataset.training_images.size() << std::endl;
    std::cout << "Nbr of training labels = " << dataset.training_labels.size() << std::endl;
    std::cout << "Nbr of test images = " << dataset.test_images.size() << std::endl;
    std::cout << "Nbr of test labels = " << dataset.test_labels.size() << std::endl;*/

	for(int i=0; i<dataset.training_images.size(); i++)
	{
		TrainingData t;
		t.DesiredOutputs = VectorXd::Zero(10);
		t.Inputs = VectorXd::Zero(28*28);

		const std::vector<uint8_t>& image = dataset.training_images[i];
		int label = (int)dataset.training_labels[i];

		for(int j=0; j<10; j++)
		{
			t.DesiredOutputs(j) = (label == j) ? 1.0 : 0.0;
		}

		for(int j=0; j<image.size(); j++)
		{
			t.Inputs(j) = (double)image[j]/255.0;
		}
		T.push_back(t);
	}
	return &T;
}


/*

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
