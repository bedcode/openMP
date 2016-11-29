/********************************************
 * Parallel code
 * it requires the number of steps
********************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static long num_steps;
double step, pi;

int main(int argc,char *argv[])
{
	int i;
	double x, sum = 0.0;

	if (argc != 2) {
		printf("Insert the number of steps\n");
		return 1;
	}

	num_steps = atol(argv[1]);
	step = 1.0/(double) num_steps;

	double initialTime = omp_get_wtime();

	#pragma omp parallel for private(x) reduction(+: sum)
		for (i=0; i < num_steps; i++) {
			x = (i + 0.5)*step;
			sum = sum + 4.0/(1.0 + x*x);
		}
	pi = step * sum;

	double finalTime = omp_get_wtime();

	printf("Pi = %1.10f\n", pi);
	printf("Time for calculation = %f\n", (finalTime - initialTime));
	return 0;
}
