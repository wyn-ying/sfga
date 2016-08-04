#include"stdafx.h"
#include <math.h>
#include "cascading.h"

void cascading(Node node[DIM], int index[], int num, double c)
{
	int flag = 0;
	vector<Node*>::iterator nbr;
	vector<Node*>::iterator nbrOfNbr;
	COST_TYPE assign_sum = 0;
	for (int i = 0; i < num; i++)
	{
		assign_sum = 0;
		for (nbr = node[index[i]].neighbor.begin(); nbr != node[index[i]].neighbor.end(); nbr++)
		{
			assign_sum += (pow((*nbr)->degree, c));
			//assign_sum += (*nbr)->degree;
		}
		for (nbr = node[index[i]].neighbor.begin(); nbr != node[index[i]].neighbor.end(); nbr++)
		{
			(*nbr)->Ci += (node[index[i]].Ci * pow((*nbr)->degree, c) / assign_sum);
			//(*nbr)->Ci += (node[index[i]].Ci * (*nbr)->degree / assign_sum);

			for (nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
			{
				if ((*nbrOfNbr) == &node[index[i]])
				{
					(*nbr)->neighbor.erase(nbrOfNbr);

					(*nbr)->degree -= 1;
					break;

				}
			}
		}// 找到该节点所有邻居点的邻居点然后去掉自己；
		node[index[i]].Ci = 0;
		node[index[i]].neighbor.clear();
	}//选择N个点去除他们的连线,并且将去除节点的容量平均分给相连的节点；
	for (int i = 0; i < DIM; i++)
	{
		if (node[i].Ci > node[i].Cm)
		{
			flag = 1;
		}
	}

	while (flag)
	{
		flag = 0;

		for (int i = 0; i < DIM; i++)
		{
			assign_sum = 0;
			if (node[i].Ci > node[i].Cm)
			{
				for (nbr = node[i].neighbor.begin(); nbr != node[i].neighbor.end(); nbr++)
				{
					assign_sum += (pow((*nbr)->degree, c));
					//assign_sum += ((*nbr)->degree);
				}
				for (nbr = node[i].neighbor.begin(); nbr != node[i].neighbor.end(); nbr++)
				{
					(*nbr)->Ci += (node[i].Ci * pow((*nbr)->degree, c) / assign_sum);
					//(*nbr)->Ci += (node[i].Ci * (*nbr)->degree / assign_sum);

					for (nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
					{
						if ((*nbrOfNbr) == &node[i])
						{
							(*nbr)->neighbor.erase(nbrOfNbr);
							(*nbr)->degree -= 1;
							break;

						}
					}

				}
				node[i].Ci = 0;
				node[i].neighbor.clear();

			}
		}
		for (int i = 0; i < DIM; i++)
		{
			if (node[i].Ci > node[i].Cm)
			{
				flag = 1;
				break;
			}
		}

	}

}
/*
#include "stdafx.h"
#include <math.h>
#include "cascading failure.h"
#include "network.h"
void cascading(Node node[NUM], int G[NUM][NUM], int index[],double a, double b)
{

	int flag=0;
	int target=index[0];//默认index是从大到小排好序的；
	vector<Node*>::iterator nbr;
	vector<Node*>::iterator nbrOfNbr;

	for(int i=0;i<NUM;i++)
	{
		node[i].Ci=pow(node[i].degree,a);//初始化节点容量；
		node[i].Cm=node[i].Ci*(1+b);//初始化最大节点容量；
	}
	double sum_degree=0;
	for (nbr = node[target].neighbor.begin(); nbr != node[target].neighbor.end(); nbr++)
	{
		sum_degree+=(*nbr)->degree;
	}
	for (nbr = node[target].neighbor.begin(); nbr != node[target].neighbor.end(); nbr++)
	{

		(*nbr)->Ci+=(node[target].Ci)*((*nbr)->degree/sum_degree);

		for ( nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
		{
			if ((*nbrOfNbr) == &node[target])
			{
				(*nbr)->neighbor.erase(nbrOfNbr);

				(*nbr)->degree -= 1;
				break;

			}
		}
	}// 找到该节点所有邻居点的邻居点然后去掉自己；
	node[target].Ci=0;
	node[target].neighbor.clear();
	for (int i=0; i<NUM; i++)
	{
		if (node[i].Ci>node[i].Cm)
		{
			flag=1;
		}
	}

	while (flag)
	{
		flag=0;

		for (int i=0; i<NUM; i++)
		{

			if (node[i].Ci>node[i].Cm)
			{
				double sum_degree=0;
				for (nbr=node[i].neighbor.begin(); nbr!=node[i].neighbor.end(); nbr++)
				{
					sum_degree+=(*nbr)->degree;
				}
				for (nbr = node[i].neighbor.begin(); nbr != node[i].neighbor.end(); nbr++)
				{

					(*nbr)->Ci+=(node[i].Ci)*((*nbr)->degree/sum_degree);


					for ( nbrOfNbr = (*nbr)->neighbor.begin(); nbrOfNbr != (*nbr)->neighbor.end(); nbrOfNbr++)
					{
						if ((*nbrOfNbr) == &node[i])
						{
							(*nbr)->neighbor.erase(nbrOfNbr);
							(*nbr)->degree -= 1;
							break;

						}
					}

				}
				node[i].Ci=0;
				node[i].neighbor.clear();

			}
		}
		for (int i=0; i<NUM; i++)
		{
			if (node[i].Ci>node[i].Cm)
			{
				flag=1;
				break;
			}
		}

	}

}
*/