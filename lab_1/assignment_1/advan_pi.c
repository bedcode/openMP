/************************************************
 * Parallel code
 * it requires the number of steps
 * implementation without the reduction clause
************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4

static long num_steps;
double step;

int main(int argc,char *argv[])
{
	int i, tid;
	double x, sum, pi = 0.0;

	if (argc != 2) {
		printf("Insert the number of steps\n");
		return 1;
	}

	num_steps = atol(argv[1]);
	step = 1.0/(double) num_steps;
	omp_set_num_threads(NUM_THREADS);

	double initialTime = omp_get_wtime();

	#pragma omp parallel private(x, sum, tid, i)
	{
		tid = omp_get_thread_num();

		for (i=tid, sum=0.0; i < num_steps; i = i + NUM_THREADS) {
			x = (i + 0.5)*step;
			sum = sum + 4.0/(1.0 + x*x);
		}

		#pragma omp atomic
			pi = pi + sum;
	}

	pi = step * pi;
	double finalTime = omp_get_wtime();

	printf("Pi = %1.10f\n", pi);
	printf("Time for calculation = %f\n", (finalTime - initialTime));
	return 0;
}
