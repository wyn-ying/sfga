// GA.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GA1.h"
using namespace std;

int main()
{
	srand(0);
	//srand((unsigned)time(NULL));
	for (int funcID = 1; funcID < 5; funcID++)
	{
		stringstream txtname;
		txtname << "GA_result_funcID=" << funcID;
#ifdef _SCALE_FREE_STATIC
		txtname << "_static";
#endif
#ifdef _SCALE_FREE_ONE
		if (M0 == POPUSIZE)
		{
			txtname << "_basic";
		}
		else
		{
			txtname << "_one";
		}

#endif
#ifdef _SCALE_FREE_DYNAMIC
		txtname << "_dynamic";
#endif
#ifdef _WITH_FITNESS_STRATEGY
		txtname << "_with_fitness_strategy";
#endif
		txtname << ".csv";
		ofstream output(txtname.str());
		GA1 ga(funcID);
		ga.output = &output;
		for (int i = 0; i < 20; i++)
		{
			cout << "The " << i + 1 << " times of " << txtname.str() << "." << endl;
			ga.Run();//test
		}
	}
	//system("pause");
    return 0;
}

