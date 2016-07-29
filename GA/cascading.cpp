#include"stdafx.h"
#include <math.h>
#include "cascading.h"

void cascading(Node node[DIM], int G[DIM][DIM], const COST_TYPE cost[DIM], const COST_TYPE sum_cost, const int index[], int num, double b)
{
	COST_TYPE sum = 0;
	int flag = 0;
	int k[DIM];
	//COST_TYPE c[DIM];//����cost;
	COST_TYPE sc = 0;
	for (int i = 0; i<DIM; i++)
	{
		k[i] = node[i].degree;
		//c[i] = pow(k[i], a);//����cost;
		sum = sum + cost[i];
	}
	for (int i = 0;i<DIM;i++)
	{
		node[i].Ci = cost[i];//��ʼ���ڵ�������
		node[i].Cm = node[i].Ci*(1 + b);//��ʼ�����ڵ�������
	}


	for (int i = 0;i<num;i++)
	{
		sc = sc + cost[index[i]];
	}


	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j<DIM; j++)
		{
			if (G[index[i]][j])
			{
				node[j].Ci = node[j].Ci + (node[index[i]].Ci / k[index[i]]);
				G[index[i]][j] = 0;
				G[j][index[i]] = G[index[i]][j];
				k[j] = k[j] - 1;
			}
		}
		node[index[i]].Ci = 0;
	}//ѡ��N����ȥ�����ǵ�����,���ҽ�ȥ���ڵ������ƽ���ָ������Ľڵ㣻
	for (int i = 0; i<DIM; i++)
	{
		if (node[i].Ci>node[i].Cm)
		{
			flag = 1;
		}
	}
	while (flag)
	{
		flag = 0;

		for (int i = 0; i < DIM; i++)
		{
			if (node[i].Ci > node[i].Cm)//����µ����������������������ô����ڵ�Ҳ����ʧЧ,���ظ�����Ĳ��裻
			{
				for (int j = 0; j < DIM; j++)
				{
					if (G[i][j])
					{
						node[j].Ci = node[j].Ci + (node[i].Ci / k[i]);
						G[i][j] = 0;
						G[j][i] = G[i][j];
						k[j] = k[j] - 1;
					}

				}
				node[i].Ci = 0;
			}

		}
		for (int i = 0; i < DIM; i++)
		{
			if (node[i].Ci > node[i].Cm)
			{
				flag = 1;
			}
		}
	}
}

void cascading(Node node[DIM], int index[], int num)
{
	int flag = 0;
	vector<Node*>::iterator nbr;
	vector<Node*>::iterator nbrOfNbr;

	for (int i = 0; i < num; i++)
	{

		for (nbr = node[index[i]].neighbor.begin(); nbr != node[index[i]].neighbor.end(); nbr++)
		{
			(*nbr)->Ci += (node[index[i]].Ci / node[index[i]].degree);

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
		if (node[i].Ci > node[index[i]].Cm)
		{
			flag = 1;
		}
	}

	while (flag)
	{
		flag = 0;

		for (int i = 0; i < DIM; i++)
		{

			if (node[i].Ci > node[i].Cm)
			{
				for (nbr = node[i].neighbor.begin(); nbr != node[i].neighbor.end(); nbr++)
				{
					(*nbr)->Ci += (node[i].Ci / node[i].degree);

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
