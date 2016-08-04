#include "stdafx.h"
#include "GA1.h"
#include<iostream>
using namespace std;

GA1::GA1(int G[DIM][DIM], COST_TYPE sum_cost, double a, double b, double c)
{
	func = *new Functions(G, sum_cost, a, b, c);
	use_heuristic_flag = 0;

}

void GA1::HeuristicInit(int pheno_heuristic[DIM])
{
	use_heuristic_flag = 1;
	for (int i = 0; i < DIM; i++)
	{
		this->pheno_heuristic[i] = pheno_heuristic[i];
	}
}
void GA1::Init()
{
	rand();
	population.clear();
	//TODO:large degree at front
	int pheno[DIM];
	LDP(pheno);
	population.push_back(new Individual(func, pheno));
	//TODO:small degree at front
	SDP(pheno);
	population.push_back(new Individual(func, pheno));
	for (int i = 0; i < POPUSIZE - 2; i++)
	{
		if (use_heuristic_flag && rand()<RAND_MAX * HEURISTIC_P)
		{
			population.push_back(new Individual(func, pheno_heuristic));
		}
		else
		{
			population.push_back(new Individual(func));
		}
	}
	ResetNetwork(population);
}

void GA1::Run(int gbest_pheno[DIM])
{
	Init();
	for (int g = 0; g < GMAX; g++)
	{
		//cout << "Now g=" << g << ", ";
		Reproduct();
#ifdef _SCALE_FREE_ONE
		ResetNetwork(population);
#endif
		gbest = population[0];
		for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
		{
			if ((*i)->funcVal< gbest->funcVal)
			{
				gbest = (*i);
			}
		}
		if (!(g%10))
		{
			int idx[DIM], num = 0;
			COST_TYPE sum = 0;
			for (int i = 0; i < DIM; i++)
			{
				idx[i] = 0;
			}
			for (int i = 0; i < DIM; i++)
			{
				if (gbest->phenotype[i] == 1)
				{
					idx[num++] = i;
				}
			}
			/*cout << "The combo to attack is: " << endl;
			for (int i = 0; i < num; i++)
			{
				cout << idx[i] << "  cost: " << func.node[idx[i]].cost << endl;
				sum += func.node[idx[i]].cost;
			}
			cout << "The total cost is: " << sum << endl;*/
			*output << gbest->funcVal << ",";
			if (gbest->funcVal == 1)
			{
				for (g+=1;g < GMAX; g++)
				{
					if (!(g % 10))
					{
						*output << gbest->funcVal << ",";
					}
				}
				break;
			}
			//cout << g << "\t";
		}
		//cout << "funcVal of gbest is:" << gbest->funcVal << endl;
	}
	*output << gbest->funcVal << ",";
	for (int i = 0; i < DIM; i++)
	{
		if (gbest->phenotype[i] == 1)
		{
			*output << "," << i;
		}
	}
	*output << endl;
	cout << "funcVal of gbest:" << gbest->funcVal << endl << endl;

	for (int i = 0; i < DIM; i++)
	{
#ifdef _USE_HEURISTIC_INIT
		gbest_pheno[i] = gbest->phenotype[i];
#endif
		pheno_heuristic[i] = gbest->phenotype[i];
	}

	Free(population);
}

void GA1::Reproduct()
{
	vector<Individual*> childPopulation;
	int child1[DIM], child2[DIM];
	Individual* parent1;
	Individual* parent2;

	for (vector<Individual*>::iterator i = population.begin(); i != population.end();i++)
	{
		(*i)->isparent = true;
	}

	for (int i = 0; i < POPUSIZE; i++)
	{
		parent1 = population[i];
		if (parent1->neighbor.size() == 0)
		{
			memcpy(child1, parent1->genotype, sizeof(unsigned long int)*DIM);
			memcpy(child2, parent1->genotype, sizeof(unsigned long int)*DIM);
			Mutate(child1);
			Mutate(child2);
		}
		else
		{
			parent2 = population[i]->neighbor[rand() % population[i]->neighbor.size()];
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
		}
		
		childPopulation.push_back(new Individual(func, child1));
		childPopulation.push_back(new Individual(func, child2));
	}
	Filtrate(childPopulation, population);
}

Individual* GA1::Select(vector<Individual*> population)
{
	double sum_p = 0, tmp_p = 0, rnd;
	vector<Individual*>::iterator i;
	for (i = population.begin(); i != population.end(); i++)
	{
		sum_p += (*i)->fitness;
	}
	rnd = (double)rand() / RAND_MAX * sum_p;
	for (i = population.begin(); i != population.end(); i++)
	{
		tmp_p += (*i)->fitness;
		if (tmp_p > rnd) return *i;
	}
	return population[population.size()-1];
}
void GA1::Cross(int parent1[DIM], int parent2[DIM], int child1[DIM], int child2[DIM])	
{
	int dptr;
	int idx1[DIM], idx2[DIM], idx1_num = 0, idx2_num = 0;
	COST_TYPE sum1 = 0, sum2 = 0;
	//Crossover
	dptr = rand() % DIM;
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
	for (int d = 0; d < DIM; d++)
	{
		if (child1[d] == 1)
		{
			idx1[idx1_num++] = d;
			sum1 += func.node[d].cost;
		}
		if (child2[d] == 1)
		{
			idx2[idx2_num++] = d;
			sum2 += func.node[d].cost;
		}
	}
	//交叉后解不可行怎么办
	int id, tmp;
	while (sum1 > func.sum_cost)
	{
		id = rand() % idx1_num;
		child1[idx1[id]] = 0;
		sum1 -= func.node[idx1[id]].cost;
		tmp = idx1[id]; idx1[id] = idx1[idx1_num - 1]; idx1[idx1_num - 1] = tmp;
		idx1_num--;
	}
	while (sum2 > func.sum_cost)
	{
		id = rand() % idx2_num;
		child2[idx2[id]] = 0;
		sum2 -= func.node[idx2[id]].cost;
		tmp = idx2[id]; idx2[id] = idx2[idx2_num - 1]; idx2[idx2_num - 1] = tmp;
		idx2_num--;
	}
}

void GA1::Mutate(int genotype[DIM])
{
	int idx[DIM], idx_num = 0;
	COST_TYPE sum = 0;
	for (int d = 0; d < DIM; d++)
	{
		if (rand() < P_MUTATE*RAND_MAX)
		{
			genotype[d] = 1 - genotype[d];
		}
		if (genotype[d] == 1)
		{
			idx[idx_num++] = d;
			sum += func.node[d].cost;
		}
	}
	//变异后解不可行怎么办
	int id, tmp;
	while (sum > func.sum_cost)
	{
		id = rand() % idx_num;
		genotype[idx[id]] = 0;
		sum -= func.node[idx[id]].cost;
		tmp = idx[id]; idx[id] = idx[idx_num - 1]; idx[idx_num - 1] = tmp;
		idx_num--;
	}
}

void GA1::Filtrate(vector<Individual*> childPopulation, vector<Individual*> &population)	//
{
	vector<Individual*> total_population = population;
	vector<Individual*> next_population;
	total_population.insert(total_population.end(), childPopulation.begin(), childPopulation.end());
	sort(total_population.begin(), total_population.end(), CmpWithFitness());

	vector<Individual*>::iterator it = total_population.begin();
	//wheel
	int fit = 1;
	Individual* s = nullptr;
	next_population.push_back(new Individual(func, total_population[0]));
	next_population[0]->fitness = fit++;
	total_population[0]->is_chosen = true;
	it++;
	for (; it != total_population.end(); it++)
	{
		(*it)->fitness = 1 / sqrt(fit++);
		(*it)->is_chosen = false;
	}
	for (int i = 1; i < POPUSIZE; i++)
	{
		s = Select(total_population);
		next_population.push_back(new Individual(func, s));
#ifndef _SCALE_FREE_ONE
		if (s->is_chosen)
		{	
			//the new individual will be marked as parent only if it is the first time to be chosen
			(*(next_population.end() - 1))->isparent = false;
		}
		s->is_chosen = true;
#endif
	}
#ifdef _SCALE_FREE_STATIC
	ReplaceinNetwork(population, next_population);
#endif
#ifdef _SCALE_FREE_DYNAMIC
	RebuildNetwork(population, next_population);
#endif

	population = next_population;
	Free(total_population);//all that flag==true
}

void GA1::ReplaceinNetwork(vector<Individual*> population, vector<Individual*> next_population)
{
	Individual* old_i = nullptr;
	//rebuild the edges of parents
	for (vector<Individual*>::iterator i = next_population.begin(); i != next_population.end(); i++)
	{
		if ((*i)->isparent)
		{
			old_i = (*i)->copy;
			for (vector<Individual*>::iterator nbr = old_i->neighbor.begin(); nbr != old_i->neighbor.end(); nbr++)
			{
				for (vector<Individual*>::iterator nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
				{
					if ((*nbrOfNbr) == old_i)
					{
						(*i)->neighbor.push_back((*nbr));
/**/						(*nbrOfNbr) = (*i);
						break;
					}
				}
			}
		}
	}
#ifdef _WITH_FITNESS_STRATEGY
	sort(next_population.begin(), next_population.end(), CmpWithFitness());
#endif

	vector<Individual*>::iterator next_it = next_population.begin();
	for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
	{
		if (!((*i)->is_chosen))
		{
			while ((*next_it)->isparent)
			{
				next_it++;
			}
			for (vector<Individual*>::iterator nbr = (*i)->neighbor.begin(); nbr != (*i)->neighbor.end(); nbr++)
			{
				for (vector<Individual*>::iterator nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
				{
					if ((*nbrOfNbr) == (*i))
					{
						(*next_it)->neighbor.push_back((*nbr));
						(*nbrOfNbr) = (*next_it);
						break;
					}
				}
			}
			next_it++;
		}
	}
}

void GA1::RebuildNetwork(vector<Individual*> population, vector<Individual*> next_population)//dynamic, average edges for newborn
{
	Individual* old_i = nullptr;
	vector<Individual*> existing_population;
	//map<vector<Individual*>::iterator, int> edge_number;
	int exist_edge_number = 0, child_number = 0, edge_for_child_i = 0;
	//rebuild the edges of parents
	for (vector<Individual*>::iterator i = next_population.begin(); i != next_population.end(); i++)
	{
		if ((*i)->isparent)
		{
			old_i = (*i)->copy;
			for (vector<Individual*>::iterator nbr = old_i->neighbor.begin(); nbr != old_i->neighbor.end(); nbr++)
			{
				if ((*nbr)->is_chosen)
				{
					for (vector<Individual*>::iterator nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
					{
						if ((*nbrOfNbr) == old_i)
						{
							for (vector<Individual*>::iterator new_nbr = next_population.begin(); new_nbr != next_population.end(); new_nbr++)
							{
								if ((*new_nbr)->copy == (*nbr))
								{
									(*i)->neighbor.push_back((*new_nbr));
									break;
								}
							}
							break;
						}
					}
				}
				
			}
			existing_population.push_back((*i));
			exist_edge_number += (*i)->neighbor.size();
		}
		else
		{
			child_number++;
		}
	}
	int total_edge_number_require = 0;
	for (vector<Individual*>::iterator i = population.begin(); i != population.end(); i++)
	{
		total_edge_number_require += (*i)->neighbor.size();
	}
	total_edge_number_require = (total_edge_number_require - exist_edge_number) / 2;
	//average
	vector<Individual*>::iterator it = population.begin();	//'it' is no use in average version
	for (vector<Individual*>::iterator i = next_population.begin(); i != next_population.end(); i++)
	{
		if (!((*i)->isparent))
		{
			while ((*it)->is_chosen)
			{
				it++;
			}
			edge_for_child_i = total_edge_number_require / child_number +
							  (total_edge_number_require % child_number > 0 ? 1 : 0);
			if (existing_population.size() <= edge_for_child_i)
			{
				for (vector<Individual*>::iterator j = existing_population.begin(); j != existing_population.end(); j++)
				{
					(*i)->neighbor.push_back((*j));
					(*j)->neighbor.push_back((*i));
					total_edge_number_require--;
				}
			}
			else
			{
				//deal with link matter
				total_edge_number_require -= edge_for_child_i;
				AddIntoNetwork(existing_population, *i, edge_for_child_i);
			}
			existing_population.push_back((*i));
			child_number--;
			it++;
		}
	}
/*	int remainer_number = total_edge_number_require % child_number, remainer_number_tmp = 0;
	for (vector<Individual*>::iterator i = next_population.begin(); i != next_population.end(); i++)
	{
		if (!((*i)->isparent))
		{
			edge_number[i] = total_edge_number_require / child_number;
		}
	}

	vector<Individual*>::iterator in_remainer = next_population.begin();
	for (; in_remainer != next_population.end(); in_remainer++)
	{
		if (!((*in_remainer)->isparent))
		{
			if (remainer_number_tmp<remainer_number)
			{
				edge_number[in_remainer] += 1;
				remainer_number_tmp++;
			}
			else
			{
				break;
			}
		}
	}

	vector<Individual*>::iterator it = population.begin();
	for (vector<Individual*>::iterator i = next_population.begin(); i != next_population.end(); i++)
	{
		if (!((*i)->isparent))
		{
			while ((*it)->is_chosen)
			{
				it++;
			}
			if (existing_population.size()<=edge_number[i])
			{
				for (vector<Individual*>::iterator j = existing_population.begin(); j != existing_population.end(); j++)
				{
					(*i)->neighbor.push_back((*j));
					(*j)->neighbor.push_back((*i));
				}
				//the rest links are added into other offsprings
				for (int dif = edge_number[i]; dif > existing_population.size(); dif--)
				{
					//TODO:if in_remainer goes to end but the 'dif' loop is not return, then some edges will be lost
					while (in_remainer != next_population.end() && (*in_remainer)->isparent)
					{
						in_remainer++;
					}
					if (in_remainer != next_population.end())
					{
						edge_number[in_remainer] += 1;
						in_remainer++;
					}
				}
			}
			else
			{
				//deal with link matter
				AddIntoNetwork(existing_population, *i, edge_number[i]);
			}
			existing_population.push_back((*i));
			it++;
		}
	}*/
}
void GA1::AddIntoNetwork(vector<Individual*> exist_population, Individual* individual, int edge_number)//dynamic
{
	double *fit = new double[exist_population.size()], fit_n = 1, sum = 0, tmp = 0, rnd = 0;
	//todo:improve: only deal with the last element
	//todo:degree version and mixed version
	//better fitness will achieve larger probability to link
	sort(exist_population.begin(), exist_population.end(), CmpWithFitness());//lower fitness is at front
	for (int i = 0; i < exist_population.size(); i++)
	{
		fit[i] = i / sqrt(fit_n++);
		sum += fit[i];
	}
	for (int l = 0; l < edge_number; l++)
	{
		tmp = 0;
		rnd = (double)rand() / RAND_MAX * sum;
		for (int i = 0; i < exist_population.size(); i++)
		{
			tmp += fit[i];
			if (tmp>rnd)
			{
				individual->neighbor.push_back(exist_population[i]);
				exist_population[i]->neighbor.push_back(individual);
				sum -= fit[i];
				fit[i] = 0;
				break;
			}
		}
	}
	delete[] fit;
}

void GA1::RemovefromNetwork(Individual* individual)//dynamic
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

void GA1::Free(vector<Individual*> population)
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

void GA1::Free(vector<Individual*>::iterator begin, vector<Individual*>::iterator end)
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

void GA1::LDP(int pheno[DIM])
{
	int idx[DIM], tmp, key;
	COST_TYPE tmp_cost;
	tmp_cost = 0;
	for (int i = 0; i < DIM; i++)
	{
		idx[i] = i;
		pheno[i] = 0;
	}
	for (int i = 0; i < DIM; i++)
	{
		key = i;
		for (int j = i; j < DIM; j++)
		{
			if (func.node[idx[j]].degree > func.node[idx[key]].degree)
			{
				key = j;
			}
		}
		tmp = idx[i]; idx[i] = idx[key]; idx[key] = tmp;
		tmp_cost += func.node[idx[i]].cost;
		pheno[idx[i]] = 1;
		if (tmp_cost > func.sum_cost)
		{
			pheno[idx[i]] = 0;
			break;
		}
	}
}

void GA1::SDP(int pheno[DIM])
{
	int idx[DIM], tmp, key;
	COST_TYPE tmp_cost;
	tmp_cost = 0;
	for (int i = 0; i < DIM; i++)
	{
		idx[i] = i;
		pheno[i] = 0;
	}
	for (int i = 0; i < DIM; i++)
	{
		key = i;
		for (int j = i; j < DIM; j++)
		{
			if (func.node[idx[j]].degree < func.node[idx[key]].degree)
			{
				key = j;
			}
		}
		tmp = idx[i]; idx[i] = idx[key]; idx[key] = tmp;
		tmp_cost += func.node[idx[i]].cost;
		pheno[idx[i]] = 1;
		if (tmp_cost > func.sum_cost)
		{
			pheno[idx[i]] = 0;
			break;
		}
	}
}