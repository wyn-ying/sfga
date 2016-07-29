// GA.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GA1.h"
#include "SGA.h"
using namespace std;

int main()
{
	srand(1);
	//srand((unsigned)time(NULL));
	double a = 1, b = 0.2;
	for (b = 0.9; b <= 1; b += 0.1)
	{
		stringstream txtname;
		txtname << "power grid b=" << b << ".csv";
		ofstream output(txtname.str());
		//TODO:传网络G进去，对固定的一个G进行优化，找到最好的组合
		//TODO:算好网络中每个节点的初始cost，传给GA，GA传给func
		int G[DIM][DIM];
		Node node[DIM];
		net::BANetworkG(G, 2, 2);
		net::SetNetwork(node, G);
		COST_TYPE sum_cost = 50;
		COST_TYPE cost[DIM];
		COST_TYPE sum = 0; 
		for (int i = 0; i < DIM; i++)
		{
			cost[i] = pow(node[i].degree, a);
		}
		GA1 ga(G, cost, sum_cost, b);
		ga.output = &output;
		for (int i = 0; i < 1; i++)
		{
			cout << "The " << i + 1 << " times of " << txtname.str() << "." << endl;
			ga.Run();//test
		}
	}
	//system("pause");
    return 0;
}

