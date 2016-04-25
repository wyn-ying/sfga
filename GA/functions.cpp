#pragma once
#include "stdafx.h"
#include "functions.h"
Functions::Functions()
{
	this->idx = 1;
	LB = -5.12; UB = 5.12;
}
Functions::Functions(int index)
{
	this->idx = index;
	switch (idx)
	{
	case 2:
		LB = -5.12; UB = 5.12; break;
	default:
		LB = 0; UB = 0; break;
	}
}
double Functions::F(double x[DIM])
{
	switch (idx)
	{
	case 2:
		return Ras(x);break;
	default:
		return 0;break;
	}
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