#include "stdafx.h"
#include "network.h"

void SetNetByBA(int G[POPUSIZE][POPUSIZE]);
void SetNetByFitness(int G[POPUSIZE][POPUSIZE], vector<Individual*> &population);

void ResetNetwork(vector<Individual*>& population)
{
	int G[POPUSIZE][POPUSIZE];
	//ScaleFreeNetwork
	for (int i = 0; i < POPUSIZE; i++)
	{
		for (int j = 0; j < POPUSIZE; j++)
		{
			G[i][j] = 0;
		}
	}
#ifdef _WITH_FITNESS_STRATEGY
	SetNetByFitness(G, population);
#else
	SetNetByBA(G);
#endif
	for (int i = 0; i < POPUSIZE; i++)
	{
		population[i]->neighbor.clear();
		for (int j = 0; j < POPUSIZE; j++)
		{
			if (G[i][j])
			{
				population[i]->neighbor.push_back(population[j]);
			}
		}
	}
//#ifdef _WITH_FITNESS_STRATEGY
//	for (int i = 0; i < POPUSIZE - 1; i++)
//	{
//		swap(population[i], population[rand() % (POPUSIZE - 1 - i) + i + 1]);
//	}
//#endif
}

void SetNetByBA(int G[POPUSIZE][POPUSIZE])
{
	int degree[POPUSIZE], degree_tmp[POPUSIZE];
	for (int i = 0; i < POPUSIZE; i++)
	{
		degree[i] = 0;
	}
	for (int i = 0; i < M0; i++)
	{
		for (int j = 0; j < M0; j++)
		{
			if (i != j)
			{
				G[i][j] = 1;
				degree[i]++;
			}
		}
	}
	int sum, sum_tmp, tmp, rnd;
	for (int i = M0; i < POPUSIZE; i++)
	{
		sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += degree[j];
		}
		for (int j = 0; j < i; j++)
		{
			degree_tmp[j] = degree[j];
		}
		sum_tmp = sum;
		for (int l = 0; l < M; l++)
		{
			rnd = rand() % sum_tmp;
			tmp = 0;
			for (int j = 0; j < i; j++)
			{
				tmp += degree_tmp[j];
				if (rnd < tmp)
				{
					G[i][j] = 1;
					G[j][i] = 1;
					sum_tmp -= degree_tmp[j];
					degree[i]++;
					degree[j]++;
					degree_tmp[j] = 0;
					break;
				}
			}
		}
	}
}

void SetNetByFitness(int G[POPUSIZE][POPUSIZE], vector<Individual*> &population)
{
	double p[POPUSIZE], p_tmp[POPUSIZE];
	sort(population.begin(), population.end(), Cmp());
	int fit = 1;
	for (int i = 0; i < POPUSIZE; i++)
	{
		population[i]->fitness = 1 / sqrt(fit++);
	}
	for (int i = 0; i < M0; i++)
	{
		for (int j = 0; j < M0; j++)
		{
			if (i != j)
			{
				G[i][j] = 1;
			}
		}
		p[i] = population[i]->fitness;
	}
	double sum, sum_tmp, tmp, rnd;
	for (int i = M0; i < POPUSIZE; i++)
	{
		p[i] = population[i]->fitness;
		sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += p[j];
		}
		for (int j = 0; j < i; j++)
		{
			p_tmp[j] = p[j];
		}
		sum_tmp = sum;
		for (int l = 0; l < M; l++)
		{
			rnd = (double)rand() / RAND_MAX * sum_tmp;
			tmp = 0;
			for (int j = 0; j < i; j++)
			{
				tmp += p_tmp[j];
				if (rnd < tmp)
				{
					G[i][j] = 1;
					G[j][i] = 1;
					sum_tmp -= p_tmp[j];
					p_tmp[j] = 0;
					break;
				}
			}
		}
	}
}

void SetNetwork(vector<Individual*>& population)
{
	for (int i=0; i < population.size(); i++)
	{
		population[i]->neighbor.push_back(population[1]);
	}
}