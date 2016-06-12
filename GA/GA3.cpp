#include "stdafx.h"
#include "GA3.h"
#include<iostream>
using namespace std;

GA3::GA3(int func_idx)
{
	func = *new Functions(func_idx);
}

void GA3::Init()
{
	rand();
	population.clear();
	vector<Individual*>().swap(population);
	for (int i = 0; i < POPUSIZE; i++)
	{
		population.push_back(new Individual(func));
	}
	ResetNetwork(population);
}

void GA3::Run()
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
			(*i)->fitness = pow(M_E, -(*i)->funcVal / (*population.begin())->funcVal - 1);
			//(*i)->fitness = population[POPUSIZE - 1]->funcVal - (*i)->funcVal;
		}
		Reproduct();
#ifdef _SCALE_FREE_ONE
		ResetNetwork(population);
#endif
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

void GA3::Reproduct()
{
	vector<Individual*> childPopulation;
	unsigned long int child1[DIM], child2[DIM];
	int rnd_i, rnd_n;
	for (vector<Individual*>::iterator i = population.begin(); i != population.end();i++)
	{
		(*i)->isparent = true;
	}

	Individual* parent1;
	Individual* parent2;

	while (childPopulation.size() < (POPUSIZE*2))
	{
		//randomly choose one from the population
		/*rnd_i = rand() % POPUSIZE;
		if (population[rnd_i]->neighbor.empty())
		{
		memcpy(child1, population[rnd_i]->genotype, sizeof(unsigned long int)*DIM);
		Mutate(child1);
		childPopulation.push_back(new Individual(func, child1));
		}
		else//randomly choose one from the first one's neighbor
		{
		rnd_n = rand() % population[rnd_i]->neighbor.size();
		if (rand() < RAND_MAX*P_CROSS)
		{
		Cross(population[rnd_i]->genotype, population[rnd_n]->genotype, child1, child2);
		}
		else
		{
		memcpy(child1, population[rnd_i]->genotype, sizeof(unsigned long int)*DIM);
		memcpy(child2, population[rnd_n]->genotype, sizeof(unsigned long int)*DIM);
		}
		Mutate(child1);
		Mutate(child2);
		childPopulation.push_back(new Individual(func, child1));
		childPopulation.push_back(new Individual(func, child2));
		}*/

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
	/*for (int i = 0; i < POPUSIZE; i++)
	{
	rnd_i = rand() % (POPUSIZE - i);
	swap(population[rnd_i], population[POPUSIZE - 1 - i]);
	}
	for (int i = 1; i < POPUSIZE; i += 2)
	{
	if (rand() < RAND_MAX*P_CROSS)
	{
	Cross(population[i]->genotype, population[i - 1]->genotype, child1, child2);
	}
	else
	{
	memcpy(child1, population[i]->genotype, sizeof(unsigned long int)*DIM);
	memcpy(child2, population[i - 1]->genotype, sizeof(unsigned long int)*DIM);
	}
	Mutate(child1);
	Mutate(child2);
	childPopulation.push_back(new Individual(func, child1));
	childPopulation.push_back(new Individual(func, child2));
	}*/

	Filtrate(childPopulation, population);

	//for (auto i :childPopulation)
	//{
	//	//TODO: check whether it is correct
	//	delete i;
	//	i = nullptr;
	//}
}

Individual* GA3::Select(vector<Individual*> population)
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
void GA3::Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM])
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
void GA3::Mutate(unsigned long int genotype[DIM])
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
void GA3::Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population)	//
{
	vector<Individual*> next_population = population, dead_population, child_dead_population;
	make_heap(next_population.begin(), next_population.end(), CmpWithFitness());
	for (vector<Individual*>::iterator child = childPopulation.begin(); child != childPopulation.end(); child++)
	{
		if ((*child)->funcVal < next_population[0]->funcVal)
		{
#ifdef _SCALE_FREE_DYNAMIC
			RemovefromNetwork(next_population[0]);
			//delete next_population[0];	////////!!!!!!!!
#endif
#ifdef _SCALE_FREE_STATIC
			if (next_population[0]->isparent)
			{
				dead_population.push_back(next_population[0]);
			}
#endif
			next_population[0] = *child;
			make_heap(next_population.begin(), next_population.end(), CmpWithFitness());
		}
		else
		{
			child_dead_population.push_back(*child);
		}
	}
#ifdef _SCALE_FREE_DYNAMIC
	AddIntoNetwork(next_population);////////看邻居的变化情况！！！！
#endif
#ifdef _SCALE_FREE_STATIC
	ReplaceinNetwork(next_population, dead_population);
#endif
	population = next_population;
	Free(child_dead_population);
}

void GA3::ReplaceinNetwork(vector<Individual*> next_population, vector<Individual*> dead_population)
{

#ifdef _WITH_FITNESS_STRATEGY
	//TODO: 2 sort(s) when coding fitness version
	sort(dead_population.begin(), dead_population.end(), CmpWithDegree());	//larger degree is at front
	sort(next_population.begin(), next_population.end(), CmpWithFitness());	//lower fitness is at front
#endif

	vector<Individual*>::iterator dead_individual = dead_population.begin();
	for (vector<Individual*>::iterator child = next_population.begin(); child != next_population.end(); child++)
	{
		if (!(*child)->isparent)
		{
			for (vector<Individual*>::iterator nbr = (*dead_individual)->neighbor.begin(); nbr != (*dead_individual)->neighbor.end(); nbr++)
			{
				for (vector<Individual*>::iterator nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
				{
					if ((*nbrOfNbr) == (*dead_individual))
					{
						(*child)->neighbor.push_back((*nbr));
						(*nbrOfNbr) = (*child);
						break;
					}
				}
			}
			dead_individual++;
		}
	}
	Free(dead_population);
}

void GA3::AddIntoNetwork(vector<Individual*>& population)//dynamic
{
	map<vector<Individual*>::iterator, int> degree, fitRank, linkProb, linkProbTmp;
	int sumProb = 0, sumProbTmp;
	////worst is at front ??
	sort(population.begin(), population.end(), CmpWithFitness());//lower fitness is at front
	int fitRankCnt = 1, rnd_l, sumProbCnt;
	for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
	{
		if ((*i)->isparent)
		{
			degree.insert(pair<vector<Individual*>::iterator, int>(i, (*i)->neighbor.size() + 1));
			fitRank.insert(pair<vector<Individual*>::iterator, int>(i, fitRankCnt++));
			linkProb.insert(pair<vector<Individual*>::iterator, int>(i, degree[i] + fitRank[i]));
			sumProb += linkProb[i];
		}
	}
	for (vector<Individual*>::iterator child = population.begin(); child != population.end(); child++)
	{
		if (!(*child)->isparent)
		{
			linkProbTmp = linkProb;
			sumProbTmp = sumProb;
			for (int l = 0; l < M; l++)//TODO: the added edges equals the edges of remove individual
			{
				sumProbCnt = 0;
				rnd_l = rand() % sumProbTmp;
				for (vector<Individual*>::iterator parent = population.begin(); parent != population.end(); parent++)
				{
					if ((*parent)->isparent)
					{
						sumProbCnt += linkProbTmp[parent];
						if (rnd_l < sumProbCnt)
						{
							sumProbTmp -= linkProbTmp[parent];
							linkProbTmp[parent] = 0;
							(*child)->neighbor.push_back((*parent));
							(*parent)->neighbor.push_back((*child));
							break;
						}
					}
				}
			}
		}
	}
}

void GA3::RemovefromNetwork(Individual* individual)//dynamic
{
	for (vector<Individual*>::iterator nbr = individual->neighbor.begin(); nbr != individual->neighbor.end(); nbr++)
	{
		//remove itself from its neighbor
		for (vector<Individual*>::iterator nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
		{
			if ((*nbrOfNbr) == individual)
			{
				(*nbr)->neighbor.erase(nbrOfNbr);
				break;
			}
		}
		//(*nbr)->neighbor.erase(&individual);
	}
}

void GA3::Free(vector<Individual*> population)
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
