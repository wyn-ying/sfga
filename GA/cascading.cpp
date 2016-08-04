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
		}// �ҵ��ýڵ������ھӵ���ھӵ�Ȼ��ȥ���Լ���
		node[index[i]].Ci = 0;
		node[index[i]].neighbor.clear();
	}//ѡ��N����ȥ�����ǵ�����,���ҽ�ȥ���ڵ������ƽ���ָ������Ľڵ㣻
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
	int target=index[0];//Ĭ��index�ǴӴ�С�ź���ģ�
	vector<Node*>::iterator nbr;
	vector<Node*>::iterator nbrOfNbr;

	for(int i=0;i<NUM;i++)
	{
		node[i].Ci=pow(node[i].degree,a);//��ʼ���ڵ�������
		node[i].Cm=node[i].Ci*(1+b);//��ʼ�����ڵ�������
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
	}// �ҵ��ýڵ������ھӵ���ھӵ�Ȼ��ȥ���Լ���
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