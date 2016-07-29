#pragma once
#include <vector>
#include "parameters.h"
#include <map>
#include <queue>
using namespace std;

class Node
{
public:
	int ID;
	int degree;
	int Q;
	int LC;
	COST_TYPE Ci;
	COST_TYPE Cm;
	vector<Node*> neighbor;
	vector<Node*> tao;
	int componentID;
};
void CopyNode(Node oldnode[DIM], Node newnode[DIM]);
void SetCapacity(Node node[DIM], double a, double b);
namespace net {
	void SetNetwork(Node node[DIM], const int G[DIM][DIM]);
	int Connectivity(Node node[DIM]);	//return the size of maximal connected subgraph
	void BANetworkG(int G[DIM][DIM], int m0, int m);
}
