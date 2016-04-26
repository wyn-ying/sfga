#pragma once
#include "stdafx.h"
#include "individual.h"
#include <algorithm>
void ResetNetwork(vector<Individual*>& population);
void SetNetwork(vector<Individual*>& individual);
class Cmp
{
public:
	bool operator()(Individual* i1, Individual* i2) { return i1->fitness < i2->fitness; };

private:

};

