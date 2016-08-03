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
	rand();
	//srand((unsigned)time(NULL));
	int G[DIM][DIM];
	net::BANetworkG(G, 2, 2);
	double a = 1.6, b = 1, c = 1, p=0.06;
	for (a = 1; a <= 2.0; a += 0.1)
	for (b = 0.1; b <= 2; b += 0.1)
	{
		stringstream txtname;
		txtname << "power grid a="<<a<<" b="<<b<<" p=0.06.csv";
		ofstream output(txtname.str(),ios::app);
		//TODO:传网络G进去，对固定的一个G进行优化，找到最好的组合
		//TODO:算好网络中每个节点的初始cost，传给GA，GA传给func
		COST_TYPE sum_cost = 0;
		int tmp = 0;
		for (int i = 0; i < DIM; i++)
		{
			for (int j = 0; j < DIM; j++)
			{
				tmp += G[i][j];
			}
		}
		sum_cost = tmp * p;
		GA1 ga(G, sum_cost, a, b, c);
		ga.output = &output;
		for (int i = 0; i < 5; i++)
		{
			cout << "The " << i + 1 << " times of " << txtname.str() << "." << endl;
			ga.Run();//test
		}
	}
	//system("pause");
    return 0;
}

