#pragma once
#include "stdafx.h"
#include "parameters.h"
#include "functions.h"
#include<time.h>
#include <vector>
using namespace std;
class Individual
{
public:
	int genotype[DIM];
	int phenotype[DIM];
	int funcVal;
	double fitness;
	bool isparent;
	bool is_chosen;
	Functions* func;
	vector<Individual*> neighbor;
	Individual* copy;
public:
	Individual(Functions &func);
	Individual(Functions &func, double phenotype[DIM]);
	Individual(Functions &func, int genotype[DIM]);
	Individual(Functions &func, Individual* individual);
	void Fit();
	void Encode();
	void Decode();
	~Individual();
};