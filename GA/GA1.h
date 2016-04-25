#pragma once
#include "stdafx.h"
#include "functions.h"
#include "individual.h"
#include "network.h"
#include "parameters.h"
#include <map>
#include <algorithm>
using namespace std;
class GA
{
public:
	Functions func;
	vector<Individual*> population;
public:
	GA(int func_idx);
	void Init();
	void Run();
	void Reproduct();
	void AddIntoNetwork();//dynamic
	void RemovefromNetwork(Individual* individual);
	vector<Individual*> Select(vector<Individual*> childPopulation);
private:
	class Compare
	{
	public:
		bool operator()(Individual* i1, Individual* i2);
	};
	void Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM]);
	void Mutate(unsigned long int genotype[DIM]);
};