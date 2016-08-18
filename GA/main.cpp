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
	srand(2);
	rand();
	//srand((unsigned)time(NULL));
	int G[DIM][DIM];
	int pheno_heuristic[50][DIM];
	net::BANetworkG(G, 2, 2);
	double a, b, c = 1, p;
	unsigned int seed = 1;
	for (int t = 0; t < 50; t++)
	{
		for (int i = 0; i < DIM; i++)
		{
			pheno_heuristic[t][i] = 0;
		}
	}
	for (a = 1.6; a <1.65; a += 0.1) {
		for (b = 0; b < 0.55; b += 0.1) {
			stringstream txtname;
			txtname << "power grid a=1.6 b=" << b << ".csv";
			ofstream output(txtname.str(), ios::app);
			for (p = 0; p < 0.201; p += 0.002)
			{
				COST_TYPE sum_cost = 0;
				/*int tmp = 0;
				for (int i = 0; i < DIM; i++)
				{
				for (int j = 0; j < DIM; j++)
				{
				tmp += G[i][j];
				}
				}	//tmp = 3994, based on parameter setting m0=m=2.*/
				sum_cost = 3994 * p;

				for (int i = 0; i < 50; i++)
				{
					srand(i);	//synchronize 50 networks
					rand();
					net::BANetworkG(G, 2, 2);
					srand(seed + unsigned(time(NULL)));	//rand, use seed+time to avoid too fast
					seed = rand();
					GA1 ga(G, sum_cost, a, b, c);
#ifdef _USE_HEURISTIC_INIT
					ga.HeuristicInit(pheno_heuristic[i]);
#endif
					ga.output = &output;
					*ga.output << p << "," << i << ",";
					cout << "The " << i + 1 << " times of " << txtname.str() << ". p=" << p << endl;
					ga.Run(pheno_heuristic[i]);//test
#ifdef _USE_HEURISTIC_INIT

#endif
				}
			}
		}
	}
	//system("pause");
	return 0;
}

