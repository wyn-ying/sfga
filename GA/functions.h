#pragma once
#include "parameters.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include <string>
#include "cascading.h"
using namespace std;

class Functions
{
public:
	double UB;
	double LB;
	int idx;
	string name;
	int G[DIM][DIM];
	double b;
	COST_TYPE sum_cost;
	COST_TYPE cost[DIM];
public:
	Functions();
	Functions(int G[DIM][DIM], COST_TYPE cost[DIM], COST_TYPE sum_cost, double b);
	Functions(int index);
	double F(double x[DIM]);
	int F(int x[DIM]);
private:
	double Griewank(double x[DIM]);
	double Ras(double x[DIM]);
	double Ackley(double x[DIM]);
	double Rosenbrock(double x[DIM]);
	double Sphere(double x[DIM]);
	double Schwefel_P2_22(double x[DIM]);
	double QuaticNoise(double x[DIM]);
};
void RndSort(int index[DIM]);