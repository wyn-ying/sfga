#include "stdafx.h"
#include "individual.h"
Individual::Individual(Functions &func)
{
	isparent = false;
	this->func = &func;
	for (int d = 0; d < DIM; d++)
	{
		phenotype[d] = (double)rand() / RAND_MAX * (this->func->UB - this->func->LB) + this->func->LB;
	}
	Encode();
	Fit();
}
Individual::Individual(Functions &func, double phenotype[DIM])
{
	isparent = false;
	this->func = &func;
	for (int d = 0; d < DIM; d++)
	{
		this->phenotype[d] = phenotype[d];
	}
	Encode();
	Fit();
}
Individual::Individual(Functions &func, unsigned long int genotype[DIM])
{
	isparent = false;
	this->func = &func;
	for (int d = 0; d < DIM; d++)
	{
		this->genotype[d] = genotype[d];
	}
	Decode();
	Fit();
}
void Individual::Fit()
{
	fitness = func->F(phenotype);
}
void Individual::Encode()
{
	for (int d = 0; d < DIM; d++)
	{
		genotype[d] = (phenotype[d] - func->LB) / (func->UB - func->LB) * 0xffffffff;
	}
}
void Individual::Decode()
{
	for (int d = 0; d < DIM; d++)
	{
		phenotype[d] = (double)genotype[d] / 0xffffffff * (func->UB - func->LB) + func->LB;
	}
}
Individual::~Individual()
{
	func = nullptr;
	for (auto i : neighbor)
	{
		i = nullptr;
		//TODO: delete this from its neighbor

	}
}
