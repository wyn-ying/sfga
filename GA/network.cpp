#include "stdafx.h"
#include "network.h"
void ResetNetwork(vector<Individual*>& population)
{
	int G[POPUSIZE][POPUSIZE];
	int degree[POPUSIZE];
	double p[POPUSIZE];
	//ScaleFreeNetwork
	for (int i = 0; i < POPUSIZE; i++)
	{
		for (int j = 0; j < POPUSIZE; j++)
		{
			G[i][j] = 0;
		}
		degree[i] = 0;
		p[i] = 0;
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
	int sum;
	double temp, rnd, p_sum;
	for (int i = M0; i < POPUSIZE; i++)
	{
		sum = 0;
		for (int j = 0; j < i; j++)
		{
			sum += degree[j];
		}
		for (int j = 0; j < i; j++)
		{
			p[j] = (double)degree[j] / sum;
		}
		temp = 0;
		for (int l = 0; l < M; l++)
		{
			for (int j = 0; j < i; j++)
			{
				p[j] = p[j] / (1 - temp);
			}
			rnd = (double)rand() / RAND_MAX,
			p_sum = 0;
			//bool flag = 1;	//consider that p_sum may be less than 1 due to numerical reason
			for (int j = 0; j < i; j++)
			{
				p_sum += p[j];
				if (rnd<p_sum)
				{
					G[i][j] = 1;
					G[j][i] = 1;
					temp = p[j];
					degree[i]++;
					degree[j]++;
					p[j] = 0;
					//flag = 0;
					break;
				}
			}
// 			if (flag)
// 			{
// 				G[i][i - 1] = 1;
// 				G[i - 1][i] = 1;
// 			}
		}
	}
	//fill individual.neighbor
	for (int i = 0; i < POPUSIZE; i++)
	{
		for (int j = 0; j < POPUSIZE; j++)
		{
			if (G[i][j])
			{
				population[i]->neighbor.push_back(population[j]);
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