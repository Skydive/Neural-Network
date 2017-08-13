#include "Network.hpp"

#include <zlib.hpp>
#include <json.hpp>
using json = nlohmann::json;

void Network::Serialize(std::string& out)
{
	json network_serial;

	for(Layer& L : Layers)
	{
		json layer_serial;
		layer_serial["Biases"] = std::vector<double>(L.Biases.data(), L.Biases.data() + L.Biases.size());
		layer_serial["Weights_Dimensions"] = {L.Weights.rows(), L.Weights.cols()};
		layer_serial["Weights"] = std::vector<double>(L.Weights.data(), L.Weights.data() + L.Weights.size());;

		network_serial.push_back(layer_serial);
	}
	out = zlib::compress_string(network_serial.dump());
}

void Network::Deserialize(const std::string& in)
{
	 std::string jsonstr = zlib::decompress_string(in);
	 auto network_serial = json::parse(jsonstr);

	 int layers = network_serial.size();
	 Layers.clear();
	 for(int i=0; i<layers; i++)
	 {
		 std::vector<double> biases = network_serial[i]["Biases"];
		 std::vector<double> weights = network_serial[i]["Weights"];
		 int w = network_serial[i]["Weights_Dimensions"][0];
		 int h = network_serial[i]["Weights_Dimensions"][1];
		 int size = network_serial[i]["Biases"].size();
		 AddLayer(size, nullptr);
		 Layers[i].Biases = Map<VectorXd, Unaligned>(biases.data(), biases.size());
		 Layers[i].Weights = Map<Matrix<double, -1, -1, ColMajor>>(weights.data(), w, h);
	 }
}
