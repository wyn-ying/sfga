// GA.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "GA1.h"
using namespace std;

int main()
{
	//srand((unsigned)time(NULL));
	cout << (double)0x7fffffff / 0xffffffff << endl;
	cout << sizeof(int);
	GA ga(2);
	ga.Run();//test
	system("pause");
    return 0;
}

