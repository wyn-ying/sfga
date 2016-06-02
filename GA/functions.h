#pragma once
#include "parameters.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include <string>
using namespace std;

class Functions
{
public:
	double UB;
	double LB;
	int idx;
	string name;
public:
	Functions();
	Functions(int index);
	double F(double x[DIM]);
private:
	double Griewank(double x[DIM]);
	double Ras(double x[DIM]);
	double Ackley(double x[DIM]);
	double Rosenbrock(double x[DIM]);
};