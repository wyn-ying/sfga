#include "stdafx.h"
#include "netnode.h"

void CopyNode(Node oldnode[DIM], Node newnode[DIM])
{
	for (int i = 0; i < DIM; i++)
	{
		newnode[i].ID = oldnode[i].ID;
		newnode[i].degree = oldnode[i].degree;
		newnode[i].Ci = oldnode[i].Ci;
		newnode[i].Cm = oldnode[i].Cm;
		newnode[i].cost = oldnode[i].cost;
		for (vector<Node*>::iterator j = oldnode[i].neighbor.begin(); j != oldnode[i].neighbor.end(); j++)
		{
			newnode[i].neighbor.push_back(&newnode[(*j)->ID]);
		}
	}
}
void SetCapacity(Node node[DIM], double a, double b)
{
	int sum = 0;
	for (int i = 0; i < DIM; i++)
	{
		node[i].Ci = pow(node[i].degree, a);
		sum = sum + node[i].Ci;
	}
	for (int i = 0;i < DIM; i++)
	{
		node[i].Ci = node[i].Ci / sum;
		node[i].Cm = node[i].Ci*(1 + b);
	}
}
void net::SetNetwork(Node node[DIM], const int G[DIM][DIM])
{
	for (int i = 0; i < DIM; i++)
	{
		node[i].ID = i;
		node[i].neighbor.clear();
		for (int j = 0; j < DIM; j++)
		{
			if (G[i][j])
			{
				node[i].neighbor.push_back(&node[j]);
				node[i].tao.push_back(&node[j]);
			}
		}
	}
	for (int i = 0; i < DIM; i++)
	{
		node[i].degree = node[i].neighbor.size();
		node[i].cost = node[i].degree;
	}
}

int net::Connectivity(Node node[DIM])
{
	int i, nodeID, maxSize = 0;
	map<int, int> componentSize;
	queue<int> q;
	for (i = 0; i < DIM; i++)
	{
		node[i].componentID = -1;
	}
	for (i = 0; i < DIM; i++)
	{
		if (node[i].componentID == -1)
		{
			node[i].componentID = i;
			componentSize[i] = 1;
			//DFS
			q.push(i);
			while (!q.empty())
			{
				nodeID = q.front();
				q.pop();
				for (auto it = node[nodeID].neighbor.begin(); it != node[nodeID].neighbor.end(); it++)
				{
					if ((*it)->componentID == -1)
					{
						(*it)->componentID = node[nodeID].componentID;
						componentSize[(*it)->componentID] += 1;
						q.push((*it)->ID);
					}
				}
			}
		}
	}
	for (auto it = componentSize.begin(); it != componentSize.end(); it++)
	{
		if ((*it).second > maxSize)
		{
			maxSize = (*it).second;
		}
	}
	return maxSize;
}

void net::BANetworkG(int G[DIM][DIM], int m0, int m)
{
	int degree[DIM], degree_tmp[DIM];
	for (int i = 0; i < DIM; i++)
	{
		degree[i] = 0;
		for (int j = 0; j < DIM; j++)
		{
			G[i][j] = 0;
		}
	}
	for (int i = 0; i < m0; i++)
	{
		for (int j = 0; j < m0; j++)
		{
			if (i != j)
			{
				G[i][j] = 1;
				degree[i]++;
			}
		}
	}
	int sum, sum_tmp, tmp, rnd;
	for (int i = m0; i < DIM; i++)
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
		for (int l = 0; l < m; l++)
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