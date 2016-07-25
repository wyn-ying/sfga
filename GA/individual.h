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
	unsigned long int genotype[DIM];
	double phenotype[DIM];
	double funcVal;
	double fitness;
	bool isparent;
	bool is_chosen;
	Functions* func;
	vector<Individual*> neighbor;
	Individual* copy;
public:
	Individual(Functions &func);
	Individual(Functions &func, double phenotype[DIM]);
	Individual(Functions &func, unsigned long int genotype[DIM]);
	Individual(Functions &func, Individual* individual);
	void Fit();
	void Encode();
	void Decode();
	~Individual();
};