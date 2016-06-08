#include "stdafx.h"
#include "GA2.h"
#include<iostream>
using namespace std;

GA0::GA0(int func_idx)
{
	func = *new Functions(func_idx);
}

void GA0::Init()
{
	rand();
	population.clear();
	vector<Individual*>().swap(population);
	for (int i = 0; i < POPUSIZE; i++)
	{
		population.push_back(new Individual(func));
	}
}

void GA0::Run()
{
	Init();
	gbest = population[0];
	for (int g = 0; g < GMAX; g++)
	{
		for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
		{
			if ((*i)->funcVal < gbest->funcVal)
			{
				gbest = (*i);
			}
		}
		sort(population.begin(), population.end(), CmpWithFitness());
		for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
		{
			(*i)->fitness = pow(M_E, -(*i)->funcVal / (*population.begin())->funcVal-1);
			//(*i)->fitness = population[POPUSIZE - 1]->funcVal - (*i)->funcVal;
		}
		Reproduct();
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

void GA0::Reproduct()
{
	vector<Individual*> childPopulation;
	unsigned long int child1[DIM], child2[DIM];

	for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
	{
		(*i)->isparent = true;
	}
	Individual* parent1;
	Individual* parent2;

	while (childPopulation.size() < CHILDSIZE)
	{
		parent1 = Select(population);
		parent2 = Select(population);
		if (rand() < RAND_MAX*P_CROSS)
		{
			Cross(parent1->genotype, parent2->genotype, child1, child2);
		}
		else
		{
			memcpy(child1, parent1->genotype, sizeof(unsigned long int)*DIM);
			memcpy(child2, parent2->genotype, sizeof(unsigned long int)*DIM);
		}
		Mutate(child1);
		Mutate(child2);
		childPopulation.push_back(new Individual(func, child1));
		childPopulation.push_back(new Individual(func, child2));
	}
	//sort(population.begin(), population.end(), CmpWithFitness());
	Filtrate(childPopulation, population,2);
	//sort(population.begin(), population.end(), CmpWithFitness());
}

Individual* GA0::Select(vector<Individual*> population)
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
		if (tmp_p > rnd) return *i;
	}
	return population[population.size() - 1];
}
void GA0::Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM])
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
void GA0::Mutate(unsigned long int genotype[DIM])
{
	int mut_tmp;
	for (int d = 0; d < DIM; d++)
	{
		mut_tmp = 0;
		for (int i = 0; i < 32; i++)
		{
			mut_tmp <<= 1;
			if (rand()<P_MUTATE*RAND_MAX)
			{
				mut_tmp += 1;
			}
		}
		genotype[d] ^= mut_tmp;
	}
}

//select POPUSIZE individuals in POPUSIZE+CHILDSIZE individuals
//use heap to implement a top K algorithm with O(N  *logK)
void GA0::Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population)	//
{
	vector<Individual*> next_population = population, dead_population, child_dead_population;
	make_heap(next_population.begin(), next_population.end(), CmpWithFitness());
	for (vector<Individual*>::iterator child = childPopulation.begin(); child != childPopulation.end(); child++)
	{
		if ((*child)->funcVal < next_population[0]->funcVal)
		{
			if (next_population[0]->isparent)
			{
				dead_population.push_back(next_population[0]);
			}
			next_population[0] = *child;
			make_heap(next_population.begin(), next_population.end(), CmpWithFitness());
		}
		else
		{
			child_dead_population.push_back(*child);
		}
	}
	population = next_population;
	Free(child_dead_population);
	Free(dead_population);
}

void GA0::Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population, int flag)	//
{
	vector<Individual*> next_population = population;
	next_population.insert(next_population.end(), childPopulation.begin(), childPopulation.end());
	sort(next_population.begin(), next_population.end(),CmpWithFitness());
	
	vector<Individual*>::iterator it = next_population.begin(), tmp = next_population.begin();
	switch (flag)
	{
	case 1:	//elitism
		for (int i = 0; i < POPUSIZE; i++)
		{
			it++;
		}
		copy(next_population.begin(), it, population.begin());
		Free(it, next_population.end());
		next_population.clear();
		next_population.swap(vector<Individual*>());
		break;
	case 2:	//tournament
		int min;
		population.clear();
		population.push_back(new Individual(func, gbest->genotype));
		gbest = population[0];
		it++;
		for (int i = 1; i < POPUSIZE; i++)
		{
			//TODO:如果population复制的是next_population中的指针，那得考虑释放内存的问题。
			//要么swap掉，要么clear掉population，再重新赋对象
			min = Tournament(next_population, CANDIDATE_SIZE);
			it = next_population.begin();
			for (int j = 0; j < min; j++)
			{
				it++;
			}
			//swap(*it, *tmp);//TODO:swap完后可以再用case 1的办法复制到population中。问题是swap导致锦标赛不能重复
			population.push_back(new Individual(func, (*it)->genotype));
		}
		
		Free(next_population);//貌似有问题
		break;
	default:
		break;
	}

}

int GA0::Tournament(vector<Individual*> population, int candidate_size)
{
	int tmp, swp, min = population.size();
	int *rank = new int[population.size()];
	for (int i = 0; i < population.size(); i++)
	{
		rank[i] = i;
	}
	for (int i = 0; i < candidate_size; i++)
	{
		swp = i + rand() % (population.size() - i);
		tmp = rank[i];
		rank[i] = rank[swp];
		rank[swp] = rank[tmp];
	}
	for (int i = 0; i < candidate_size; i++)
	{
		if (rank[i] < min)
		{
			min = rank[i];
		}
	}
	delete [] rank;
	return min;
}
void GA0::Free(vector<Individual*> population)
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

void GA0::Free(vector<Individual*>::iterator begin, vector<Individual*>::iterator end)
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

