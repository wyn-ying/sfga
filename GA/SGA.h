#pragma once
#include "stdafx.h"
#include "functions.h"
#include "individual.h"
#include "network.h"
#include "parameters.h"
#include <map>
#include <algorithm>
#include <fstream>
using namespace std;
class GA
{
public:
	Functions func;
	vector<Individual*> population;
	Individual* gbest;
	ofstream* output;
public:
	GA(int func_idx);
	void Init();
	void Run();
	void Reproduct();
	void Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population, int flag);
	void Free(vector<Individual*> population);
	void Free(vector<Individual*>::iterator begin, vector<Individual*>::iterator end);
private:
	class CmpWithFitness
	{
	public:
		bool operator()(Individual* i1, Individual* i2) { return i1->funcVal < i2->funcVal; };
	};
	class CmpWithDegree
	{
	public:
		bool operator()(Individual* i1, Individual* i2) { return i1->neighbor.size() > i2->neighbor.size(); };
	};
	Individual* Select(vector<Individual*> population);
	void Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM]);
	void Mutate(unsigned long int genotype[DIM]);
};