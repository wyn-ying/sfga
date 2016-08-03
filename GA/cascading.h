#pragma once
#include "parameters.h"
#include "netnode.h"
void cascading(Node node[DIM], int G[DIM][DIM], const COST_TYPE cost[DIM], const COST_TYPE sum_cost, const int index[], int num, double b);
void cascading(Node node[DIM], int index[], int num, double c);