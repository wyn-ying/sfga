#include "stdafx.h"
#include "individual.h"
Individual::Individual(Functions &func)
{
	copy = nullptr;
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
Individual::Individual(Functions &func, unsigned long int genotype[DIM])
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
		genotype[d] = unsigned long((phenotype[d] - func->LB) / (func->UB - func->LB) * 0xffffffff);
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
	copy = nullptr;
	func = nullptr;
	for (auto i : neighbor)
	{
		i = nullptr;
		//TODO: delete this from its neighbor
	}
}
