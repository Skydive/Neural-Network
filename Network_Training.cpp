#include "Network.hpp"

// Stochastic gradient descent
void Network::SGD(std::vector<TrainingData>& trainingdata, int epochs, int size, double eta)
{
	for(int E=0; E<epochs; E++)
	{
		std::vector<TrainingData*> pointdata;
		for(int i=0; i<trainingdata.size(); i++)
		{
			pointdata.push_back(&trainingdata[i]);
		}
		std::random_shuffle(pointdata.begin(), pointdata.end());
		for(int i=0; i<pointdata.size(); i += size)
		{
			// For each mini-batch
			std::vector<TrainingData*> minibatch;
			for(int j=0; j<size; j++)
			{
				if(i+j < pointdata.size())
				{
					minibatch.push_back(pointdata[i+j]);
				}
			}
			UpdateMiniBatch(minibatch, eta);
		}
	}
}

void Network::UpdateMiniBatch(const std::vector<TrainingData*>& minibatch, double eta)
{
	for(int i=1; i<Layers.size(); i++)
	{
		Layers[i].d_nabla_b.resize(Layers[i].Biases.size());
		Layers[i].d_nabla_w.resize(Layers[i].Weights.rows(), Layers[i].Weights.cols());
		Layers[i].d_nabla_b.setZero();
		Layers[i].d_nabla_w.setZero();
	}

	for(int i=0; i<minibatch.size(); i++)
	{
		SetInput(minibatch[i]->Inputs, nullptr);

		// BACKPROPAGATION:
		// Feedforward
		FeedForward();
		// Output Error
		Layer& OutputLayer = Layers[Layers.size()-1];
		VectorXd CostDerivative = OutputLayer.Outputs - minibatch[i]->DesiredOutputs;
		OutputLayer.Error = CostDerivative.cwiseProduct(OutputLayer.WeighedInputs.unaryExpr(DERIVATIVE_SIGMOID));
		OutputLayer.d_nabla_b += OutputLayer.Error;
		MatrixXd A = OutputLayer.Error * Layers[Layers.size()-2].Outputs.transpose();
		OutputLayer.d_nabla_w += OutputLayer.Error * Layers[Layers.size()-2].Outputs.transpose();
		// Backpropagate the error
		for(int i=Layers.size()-2; i>=1; i--)
		{
			Layer& CurrLayer = Layers[i];
			Layer& PrevLayer = Layers[i-1];
			Layer& NextLayer = Layers[i+1];
			CurrLayer.Error = (NextLayer.Weights.transpose() * NextLayer.Error).cwiseProduct(CurrLayer.WeighedInputs.unaryExpr(DERIVATIVE_SIGMOID));
			CurrLayer.d_nabla_b += CurrLayer.Error;
			CurrLayer.d_nabla_w += CurrLayer.Error * PrevLayer.Outputs.transpose();
		}
	}

	for(int i=1; i<Layers.size(); i++)
	{
		Layer& CurrLayer = Layers[i];
		CurrLayer.Weights = (CurrLayer.Weights - (eta/minibatch.size())*CurrLayer.d_nabla_w);
		CurrLayer.Biases = (CurrLayer.Biases - (eta/minibatch.size())*CurrLayer.d_nabla_b);
	}
}
