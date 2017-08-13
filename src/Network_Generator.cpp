#include "Network.hpp"

#include <dlfcn.h>
#include <cstdlib>
#include "generator/Generator.hpp"

void Network::GenerateTrainingData(const std::string& lib)
{
	CloseGenerator();

	std::cout << "Loading Shared Library: " << lib << std::endl;
	ghandle = dlopen(lib.c_str(), RTLD_LAZY);
	if(ghandle == nullptr)
	{
		std::cout << "\tError loading library: " << lib << std::endl;
		exit(1);
	}
	Generator* (*create)();
	create = (Generator* (*)())dlsym(ghandle, "create_object");

	std::cout << "\tGenerating training data..." << std::endl;
	Generator* G = (Generator*)create();
	T = G->GenerateTrainingData();
	std::cout << "\tGeneration completed..." << std::endl;

	gen = (void*)G;
}

void Network::CloseGenerator()
{
	if(ghandle != nullptr)
	{
		void (*destroy)(Generator*);
		destroy = (void (*)(Generator*))dlsym(ghandle, "destroy_object");
		if(gen != nullptr)
			destroy((Generator*)gen);
		dlclose(ghandle);
	}
}
