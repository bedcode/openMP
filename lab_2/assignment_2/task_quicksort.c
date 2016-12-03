/**************************
 * Parallel code: task
**************************/
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "omp.h"
#define DEPTH_MAX 4
#define DEPTH_MIN 1

void quicksort(int a[], int lower, int upper, int depth);
int split(int a[], int lower, int upper);


int main(int argc, char *argv[])
{
	int arr[100];
	int i, j;
	double end, start = omp_get_wtime();

	srand(time(NULL));
	#pragma omp parallel for private(j) schedule(static, 10)
		for (j = 0; j < 100; j++) {
			arr[j] = j*rand()*0.00001;
		}

	#pragma omp barrier
	printf("Full array\n");

	#pragma omp parallel
	#pragma omp single
	{
		quicksort (arr, 0, 99, DEPTH_MIN);
	}

	end = omp_get_wtime();

	printf("Calculation time: %f\n", (end - start));
	printf("\nArray after sorting:\n");

	for (i = 0; i <= 99; i++)
		printf("%d\t", arr[i]);

	return 0;
}


void quicksort (int a[], int lower, int upper, int depth)
{
	int i;
	if (upper > lower)
	{
		i = split(a, lower, upper);
		#pragma omp task if (depth > DEPTH_MAX)
			quicksort(a, lower, i - 1 , depth +1);

		#pragma omp task if (depth > DEPTH_MAX)
			quicksort(a, i + 1, upper , depth+1);
	}
	#pragma omp taskwait
}


int split(int a[], int lower, int upper)
{
	int i, p, q, t;

	p = lower + 1;
	q = upper;
	i = a[lower];

	while (q >= p)
	{
		while (a[p] < i)
			p++;

		while (a[q] > i)
			q--;

		if (q > p)
		{
			t = a[p];
			a[p] = a[q];
			a[q] = t;
		}
	}

	t = a[lower];
	a[lower] = a[q];
	a[q] = t;

	return q;
}
