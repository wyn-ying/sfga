#pragma once
#include "stdafx.h"
#include "functions.h"
Functions::Functions()
{
	this->idx = 1;
	LB = -5.12; UB = 5.12;
}
Functions::Functions(int G[DIM][DIM], COST_TYPE sum_cost, double a, double b, double c)
{
	this->sum_cost = sum_cost;
	net::SetNetwork(node, G);
	SetCapacity(node, a, b);
	this->c = c;
}

Functions::Functions(int index)
{
	this->idx = index;
	switch (idx)
	{
	case 1:
		name = "Griewank"; LB = -600; UB = 600; break;
	case 2:
		name = "Rastrigin"; LB = -15; UB = 15; break;
	case 3:
		name = "Rosenbrock"; LB = -15; UB = 15; break;
	case 4:
		name = "Ackley"; LB = -32; UB = 32; break;
	case 5:
		name = "Sphere"; LB = -100; UB = 100; break;
	case 6:
		name = "Schwefel_P2_22"; LB = -10; UB = 10; break;
	case 7:
		name = "QuaticNoise"; LB = -1.28; UB = 1.28; break;
	default:
		LB = 0; UB = 0; break;
	}
}
/*double Functions::F(double x[DIM])
{
	switch (idx)
	{
	case 1:
		return Griewank(x); break;
	case 2:
		return Ras(x); break;
	case 3:
		return Rosenbrock(x); break;
	case 4:
		return Ackley(x); break;
	case 5:
		return Sphere(x); break;
	case 6:
		return Schwefel_P2_22(x); break;
	case 7:
		return QuaticNoise(x); break;
	default:
		return 0;break;
	}
}*/

int Functions::F(int x[DIM])
{
	int G[DIM][DIM];
	int lagest_component_size;
	Node node[DIM];
	CopyNode(this->node, node);	//TODO:初始化的时候也可以做点优化，G拷贝的时候可以memcpy，node初始化的时候可以用一组原始的node[DIM]拷贝，这样就可以把第一次的Setnetwork省略掉
	int idx[DIM];
	int idx_num = 0;
	for (int i = 0; i < DIM; i++)
	{
		if (x[i] == 1)
		{
			idx[idx_num] = i;
			idx_num += 1;
		}
	}
	cascading(node, idx, idx_num, c);
	lagest_component_size = net::Connectivity(node);
	return lagest_component_size;
}

double Functions::Griewank(double x[DIM])
{
	double f = 0, f1 = 0, f2 = 1;
	int i;
	for (i = 0;i < DIM; i++)
	{
		f1 = x[i] * x[i] + f1;
		f2 = cos(x[i] / sqrt((double)i + 1))*f2;
	}
	f = (f1 / 4000) - f2 + 1;
	return f;
}

double Functions::Ras(double x[DIM])
{
	double f = 0;
	for (int d = 0; d < DIM; d++)
	{
		f += x[d] * x[d] - 10 * cos(2 * M_PI*x[d]) + 10;
	}
	return f;
}
double Functions::Ackley(double x[DIM])
{
	double f = 0, f1 = 0, f2 = 0;
	int i;
	for (i = 0;i <= DIM - 1;i++)
	{
		f1 = f1 + pow(x[i], 2);
		f2 = f2 + cos(2 * M_PI*x[i]);
	}
	f1 = f1 / DIM;
	f2 = f2 / DIM;
	f1 = -0.2*sqrt(f1);
	f = -20 * exp(f1) - exp(f2) + 20 + exp((double)1);
	return f;
}

double Functions::Rosenbrock(double x[DIM])
{
	double f = 0;
	int i;
	for (i = 0;i != DIM - 1;++i)
		f = f + (100 * (x[i + 1] - x[i] * x[i])*(x[i + 1] - x[i] * x[i]) + (x[i] - 1)*(x[i] - 1));
	return f;
}

double Functions::Sphere(double x[DIM])
{
	double f = 0;
	for (int d = 0; d < DIM; d++)
	{
		f = f + x[d] * x[d];
	}
	return f;
}

double Functions::Schwefel_P2_22(double x[DIM])
{
	double fx = 0, fx1 = 0, fx2 = 1;
	int d;
	for (d = 0;d < DIM;d++) {
		fx1 = fx1 + abs(x[d]);
		fx2 = fx2*abs(x[d]);
	}
	fx = fx1 + fx2;
	return fx;
}
double Functions::QuaticNoise(double x[DIM])
{
	double f = 0;
	for (int d = 0;d < DIM ; d++)
	{
		f = f + (d + 1)*pow(x[d], 4);
	}
	f = f + (double)rand()/RAND_MAX;
	return f;
}
void RndSort(int index[DIM])
{
	int tmp, rnd;
	for (int i = 0; i < DIM - 1; i++)
	{
		rnd = rand() % (DIM - i);
		tmp = index[i + rnd];
		index[i + rnd] = index[i];
		index[i] = tmp;
	}
}
void RndSort(int index[DIM], int num)
{
	int tmp, rnd;
	for (int i = 0; i < num - 1; i++)
	{
		rnd = rand() % (num - i);
		tmp = index[i + rnd];
		index[i + rnd] = index[i];
		index[i] = tmp;
	}
}