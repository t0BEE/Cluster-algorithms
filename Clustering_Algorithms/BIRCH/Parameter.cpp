#include "Parameter.h"

void extern addTwoPoints(double* dOne, double* dTwo)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		dOne[i] = dOne[i] + dTwo[i];
	}
}

double extern calcDistance(double* dOne, double* dTwo)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		retValue += pow(dOne[i] - dTwo[i], 2.0);
	}
	return sqrt(retValue);
}
