#include "stdafx.h"
#include "GA1.h"

GA::GA(int func_idx)
{
	func = *new Functions(func_idx);	
}

void GA::Init()
{
	rand();
	for (int i = 0; i < POPUSIZE; i++)
	{
		population.push_back(new Individual(func));
		population[i]->Fit();
	}
	InitNetwork(population);
}

void GA::Run()
{
	Init();
	for (int g = 0; g < GMAX; g++)
	{
		Reproduct();
	}
}

void GA::Reproduct()
{
	vector<Individual*> childPopulation;
	unsigned long int child1[DIM], child2[DIM];
	int rnd_i, rnd_n;
	for (vector<Individual*>::iterator i = population.begin(); i != population.end();i++)
	{
		(*i)->isparent = true;
	}

	while (childPopulation.size() < CHILDSIZE)
	{
		//randomly choose one from the population
		rnd_i = rand() % POPUSIZE;
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
		}
		
	}
	population = Select(childPopulation);
	AddIntoNetwork();////////看邻居的变化情况！！！！
	//for (auto i :childPopulation)
	//{
	//	//TODO: check whether it is correct
	//	delete i;
	//	i = nullptr;
	//}
}

void GA::Cross(unsigned long int parent1[DIM], unsigned long int parent2[DIM], unsigned long int child1[DIM], unsigned long int child2[DIM])
{
	int dptr, bptr;
	unsigned long int tmp;
	//Crossover
	dptr = rand() % DIM;
	bptr = rand() % sizeof(unsigned long int);
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
bool GA::Compare::operator()(Individual* i1, Individual* i2)
{
	return i1->fitness < i2->fitness;
}
//use heap to implement a top K algorithm with O(N  *logK)
vector<Individual*> GA::Select(vector<Individual*> childPopulation)	//
{
	vector<Individual*> next_population = population;
	make_heap(next_population.begin(), next_population.end(), Compare());
	for (vector<Individual*>::iterator tmp_c = childPopulation.begin(); tmp_c != childPopulation.end(); tmp_c++)
	{
		if ((*tmp_c)->fitness < next_population[0]->fitness)
		{
			//DOTO: the neighbor of tmp_c should be fixed::dynamic strategy has been implemented, not here
			//DOTO: the individual which connects with tmp_pop[0] should be fixed::dynamic strategy has been implemented
			RemovefromNetwork(next_population[0]);
			next_population[0] = *tmp_c;
			make_heap(next_population.begin(), next_population.end(), Compare());
		}
	}
	return next_population;
}

void GA::AddIntoNetwork()//dynamic
{
	map<vector<Individual*>::iterator, int> degree, fitRank, linkProb, linkProbTmp;
	int sumProb = 0, sumProbTmp;
	//worst is at front
	sort(population.begin(), population.end(), Compare());
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
			for (int l = 0; l < M; l++)
			{
				sumProbCnt = 0;
				sumProbTmp = sumProb;
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

void GA::RemovefromNetwork(Individual* individual)//dynamic
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
