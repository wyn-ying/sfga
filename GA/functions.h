#pragma once
#include "parameters.h"
#define _USE_MATH_DEFINES
#include<math.h>
class Functions
{
public:
	double UB;
	double LB;
	int idx;
public:
	Functions();
	Functions(int index);
	double F(double x[DIM]);
private:
	double Ras(double x[DIM]);
};