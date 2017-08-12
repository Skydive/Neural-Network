#pragma implement Network Network_Serialize Network_Training
#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>

#include <iostream>

#include <Eigen/Core>
using namespace Eigen;

const auto ACTIVATOR_PERCEPTRON = [](double z) {
	return (z >= 0) ? 1.0 : 0.0;
};
const auto ACTIVATOR_SIGMOID = [](double z) {
	return 1/(1+exp(-z));
};
const auto ACTIVATOR_SIGMOID_TANH = [](double z) {
	return (1/2)*(1+tanh(z));
};
const auto DERIVATIVE_SIGMOID = [&](double z) {
	return ACTIVATOR_SIGMOID(z)*(1-ACTIVATOR_SIGMOID(z));
};


struct TrainingData
{
	VectorXd Inputs;
	VectorXd DesiredOutputs;
};

struct Network;
struct Layer
{
	Network* ParentNetwork;
	int Index;

	MatrixXd Weights;
	VectorXd Biases;
	VectorXd Outputs;

	// Training
	VectorXd WeighedInputs;
	VectorXd Error;
	VectorXd d_nabla_b;
	MatrixXd d_nabla_w;
};

struct Network
{
	std::mt19937 mt;
	std::vector<Layer> Layers;
	Network();

	void AddLayer(int size, std::function<void(Layer&, double&, double&)> ActionNeuron);
	void SetInput(const VectorXd& v, std::function<double(double)> PrepareFunc);
	void FeedForward();
	const VectorXd& GetOutput();
	void PrintOutput();
	// Training
	void SGD(std::vector<TrainingData>& trainingdata, int epochs, int size, double eta);
	void UpdateMiniBatch(const std::vector<TrainingData*>& minibatch, double eta);
	// Serialize
	void Serialize(std::string& out);
};


const auto NEURON_GAUSSIAN = [&](Layer& L, double& Bias, double& Weight){
	auto& mt = L.ParentNetwork->mt;
	std::normal_distribution<double> N(0.0, 1.0);
	std::normal_distribution<double> B(0.0, 1/L.Outputs.size());
	Bias = B(mt);
	Weight = N(mt);
};
