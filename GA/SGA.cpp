#include "stdafx.h"
#include "SGA.h"
#include<iostream>
using namespace std;

GA::GA(int func_idx)
{
	func = *new Functions(func_idx);
}

void GA::Init()
{
	rand();
	population.clear();
	for (int i = 0; i < POPUSIZE; i++)
	{
		population.push_back(new Individual(func));
	}
}

void GA::Run()
{
	Init();
	for (int g = 0; g < GMAX; g++)
	{
		sort(population.begin(), population.end(), CmpWithFitness());
		int fit = 1;
		Reproduct();
		gbest = population[0];
		for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
		{
			if ((*i)->funcVal < gbest->funcVal)
			{
				gbest = (*i);
			}
		}
		if (!(g % 10))
		{
			*output << gbest->funcVal << ",";
			cout << g << "\t";
		}
	}
	*output << gbest->funcVal << endl;
	cout << endl << "funcVal of gbest:" << gbest->funcVal << endl;
	Free(population);
}

void GA::Reproduct()
{
	vector<Individual*> child_population;
	unsigned long int child1[DIM], child2[DIM];

	Individual* parent1;
	Individual* parent2;

	for (int i = 0; i < POPUSIZE; i++)
	{
		parent1 = population[i];
		parent2 = population[rand() % POPUSIZE];
		if (rand() < RAND_MAX*P_CROSS)
		{
			Cross(parent1->genotype, parent2->genotype, child1, child2);
		}
		else
		{
			memcpy(child1, parent1->genotype, sizeof(unsigned long int)*DIM);
			memcpy(child2, parent2->genotype, sizeof(unsigned long int)*DIM);
			Mutate(child1);
			Mutate(child2);
		}

		child_population.push_back(new Individual(func, child1));
		child_population.push_back(new Individual(func, child2));
	}
	//sort(population.begin(), population.end(), CmpWithFitness());
	Filtrate(child_population, population, 3);
	//sort(population.begin(), population.end(), CmpWithFitness());
}

Individual* GA::Select(vector<Individual*> population)
{
	double sum_p = 0, tmp_p = 0, rnd;
	vector<Individual*>::iterator i;
	for (i = population.begin(); i != population.end(); i++)
	{
		sum_p += (*i)->fitness;//TODO: wheel selection
	}
	rnd = (double)rand() / RAND_MAX * sum_p;
	for (i = population.begin(); i != population.end(); i++)
	{
		tmp_p += (*i)->fitness;
		if (tmp_p > rnd)
			return *i;
	}
	return population[population.size() - 1];
}
void GA::Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM])
{
	int dptr, bptr;
	unsigned long int tmp;
	//Crossover
	dptr = rand() % DIM;
	bptr = rand() % (sizeof(unsigned long int) * 8);
	//cross the first [dptr] genes while the copy rest genes except the [dptr]th gene
	for (int d = 0; d < dptr; d++)
	{
		child2[d] = parent1[d];
		child1[d] = parent2[d];
	}
	for (int d = dptr; d < DIM; d++)
	{
		child1[d] = parent1[d];
		child2[d] = parent2[d];
	}
	//cross the first [bptr] bits of the [dptr]th gene
	tmp = 0xffffffff << bptr;
	tmp &= child1[dptr] ^ child2[dptr];
	child1[dptr] ^= tmp;
	child2[dptr] ^= tmp;
}
void GA::Mutate(unsigned long int genotype[DIM])
{
	int mut_tmp;
	for (int d = 0; d < DIM; d++)
	{
		mut_tmp = 0;
		for (int i = 0; i < sizeof(unsigned long int) * 8; i++)
		{
			mut_tmp <<= 1;
			if (rand() < P_MUTATE*RAND_MAX)
			{
				mut_tmp += 1;
			}
		}
		genotype[d] ^= mut_tmp;
	}
}

void GA::Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population, int flag)	//
{
	vector<Individual*> next_population = population;
	next_population.insert(next_population.end(), childPopulation.begin(), childPopulation.end());
	sort(next_population.begin(), next_population.end(), CmpWithFitness());

	vector<Individual*>::iterator it = next_population.begin();
	//wheel
	int fit = 1;
	Individual* s = NULL;
	population.clear();
	//	population.push_back(new Individual(func, next_population[0]->genotype));
	//	population[0]->fitness = fit++;
	//	it++;
	for (; it != next_population.end(); it++)
	{
		(*it)->fitness = 1 / sqrt(fit++);
	}
	for (int i = 0; i < POPUSIZE; i++)
	{
		s = Select(next_population);
		population.push_back(new Individual(func, s->genotype));
	}
	Free(next_population);
}

void GA::Free(vector<Individual*> population)
{
	for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
	{
		if (*i != NULL)
		{
			delete *i;
			*i = NULL;
		}
	}
	population.clear();
	population.swap(vector<Individual*>());
}

void GA::Free(vector<Individual*>::iterator begin, vector<Individual*>::iterator end)
{
	for (vector<Individual*>::iterator i = begin; i != end; i++)
	{
		if (*i != NULL)
		{
			delete *i;
			*i = NULL;
		}
	}
}

