/******************************************************************************
* FILE: omp_reduction.c
* DESCRIPTION:
*   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
*   This example demonstrates a sum reduction within a combined parallel loop
*   construct.  Notice that default data element scoping is assumed - there
*   are no clauses specifying shared or private variables.  OpenMP will
*   automatically make loop index variables private within team threads, and
*   global variables shared.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100

int main (int argc, char *argv[])
{
	int i;
	float a[N], b[N], sum;

	/* Some initializations */
	for (i=0; i < N; i++)
		a[i] = b[i] = i * 1.0;
	sum = 0.0;

	#pragma omp parallel for reduction(+:sum)
		for (i=0; i < N; i++)
			sum = sum + (a[i] * b[i]);

	printf("Sum = %f\n", sum);
	return 0;
}
