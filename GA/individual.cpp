#include "stdafx.h"
#include "individual.h"
Individual::Individual(Functions &func)
{
	copy = nullptr;
	isparent = false;
	this->func = &func;
	int idx[DIM];
	for (int d = 0; d < DIM; d++)
	{
		idx[d] = d;
	}
	RndSort(idx);
	COST_TYPE tmp_cost = 0;
	int d;
	for (d = 0; d < DIM; d++)
	{
		phenotype[idx[d]] = rand() % 2;
		if (phenotype[idx[d]] == 1)
		{
			tmp_cost += this->func->node[idx[d]].cost;
			if (tmp_cost>this->func->sum_cost)
			{
				phenotype[idx[d]] = 0;
			}
		}
	}
	for (; d < DIM; d++)
	{
		phenotype[idx[d]] = 0;
	}
	Encode();
	Fit();
}
Individual::Individual(Functions &func, double phenotype[DIM])
{
	copy = nullptr;
	isparent = false;
	this->func = &func;
	for (int d = 0; d < DIM; d++)
	{
		this->phenotype[d] = phenotype[d];
	}
	Encode();
	Fit();
}
Individual::Individual(Functions &func, int genotype[DIM])
{
	copy = nullptr;
	isparent = false;
	this->func = &func;
	for (int d = 0; d < DIM; d++)
	{
		this->genotype[d] = genotype[d];
	}
	Decode();
	Fit();
}
Individual::Individual(Functions &func, Individual* individual)
{
	copy = individual;
	isparent = individual->isparent;
	memcpy(genotype, individual->genotype, sizeof(genotype));
	memcpy(phenotype, individual->phenotype, sizeof(phenotype));
	funcVal = individual->funcVal;
	this->func = &func;
}
void Individual::Fit()
{
	funcVal = func->F(phenotype);
}
void Individual::Encode()
{
	for (int d = 0; d < DIM; d++)
	{
		genotype[d] = phenotype[d];
	}
}
void Individual::Decode()
{
	for (int d = 0; d < DIM; d++)
	{
		phenotype[d] = genotype[d];
	}
}
Individual::~Individual()
{
	copy = nullptr;
	func = nullptr;
	for (auto i : neighbor)
	{
		i = nullptr;
		//TODO: delete this from its neighbor
	}
}
