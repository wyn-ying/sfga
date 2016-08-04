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
class GA1
{
public:
	Functions func;
	vector<Individual*> population;
	Individual* gbest;
	ofstream* output;
private:
	int pheno_heuristic[DIM];
	int use_heuristic_flag;
public:
	GA1(int G[DIM][DIM], COST_TYPE sum_cost, double a, double b, double c);
	void HeuristicInit(int pheno_heuristic[DIM]);
	void Init();
	void Run(int gbest_pheno[DIM]);
	void Reproduct();
	void AddIntoNetwork(vector<Individual*> exist_population, Individual* individual, int edge_number);//dynamic
	void RemovefromNetwork(Individual* individual);
	void ReplaceinNetwork(vector<Individual*> population, vector<Individual*> next_population);
	void RebuildNetwork(vector<Individual*> population, vector<Individual*> next_population);
	void Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population);
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
	Individual* GA1::Select(vector<Individual*> population);
	void Cross(int parent1[DIM], int parent2[DIM], int child1[DIM], int child2[DIM]);
	void Mutate(int genotype[DIM]);
	void LDP(int phenomenon[DIM]);
	void SDP(int phenomenon[DIM]);
};
