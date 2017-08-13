#include "Network.hpp"

Network::Network()
{
	ghandle = nullptr;
	gen = nullptr;
	mt = std::mt19937();
}

void Network::AddLayer(int size, std::function<void(Layer&, double&, double&)> ActionNeuron)
{
	Layers.push_back(Layer());
	Layer& NewLayer = Layers[Layers.size()-1];
	NewLayer.Index = Layers.size()-1;
	NewLayer.ParentNetwork = this;

	NewLayer.Outputs.resize(size);
	NewLayer.WeighedInputs.resize(size);
	NewLayer.Biases.resize(size);
	if(Layers.size() >= 2)
	{
		Layer& PrevLayer = Layers[Layers.size()-2];
		NewLayer.Weights.resize(size, PrevLayer.Outputs.size());
		if(ActionNeuron != nullptr)
		{
			for(int j=0; j<NewLayer.Biases.size(); j++)
			{
				for(int k=0; k<PrevLayer.Outputs.size(); k++)
				{
					ActionNeuron(NewLayer, NewLayer.Biases(j), NewLayer.Weights(j, k));
				}
			}
		}
	}
}

void Network::SetInput(const VectorXd& v, std::function<double(double)> PrepareFunc)
{
	Layer& InputLayer = Layers[0];
	InputLayer.Outputs = (PrepareFunc != nullptr) ? v.unaryExpr(PrepareFunc) : v;
}

void Network::FeedForward()
{
	for(int i=0; i<Layers.size()-1; i++)
	{
		Layer& PrevLayer = Layers[i];
		Layer& CurrLayer = Layers[i+1];

		CurrLayer.WeighedInputs = CurrLayer.Weights*PrevLayer.Outputs + CurrLayer.Biases;
		CurrLayer.Outputs = CurrLayer.WeighedInputs.unaryExpr(ACTIVATOR_SIGMOID);
	}
}

const VectorXd& Network::GetOutput()
{
	Layer& OutputLayer = Layers[Layers.size()-1];
	return OutputLayer.Outputs;
}

void Network::PrintOutput(bool bFancy)
{
	VectorXd SanitizedOutputs = GetOutput().unaryExpr(SANITIZE_OUTPUT);
	if(bFancy)
	{
		for(int i=0; i<SanitizedOutputs.size(); i++)
		{
			std::cout << "[" << i << "] -> " << SanitizedOutputs(i) << std::endl;
		}
	}
	else
	{
		std::cout << SanitizedOutputs.transpose() << std::endl;
	}
}
