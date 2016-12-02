#include <stdio.h>
#include <omp.h>

static long num_steps = 100000000; //100 M
double step;

void compute_pi(int nthreads)
{
	double x, pi, sum = 0.0;
	double end, start = omp_get_wtime();
	int i;

	#pragma omp parallel for private(x) reduction(+:sum)
	for (i=0; i<num_steps; i++) {
		x = (i+0.5)*step;
		sum = sum + 4.0/(1.0 + x*x);
	}
	pi = step * sum;
	end = omp_get_wtime();

	printf("Num threads = %d.\n", nthreads);
	printf("Elapsed time = %f sec\n", end-start);
	printf("Calculated PI = %.10f\n\n", pi);
}

int main(int argc, char *argv[])
{
	step = 1.0/ (double) num_steps;
	int i;

	for (i=1; i<=8; i*=2) {
		omp_set_num_threads(i);
		compute_pi(i);
	}

	return 0;
}
