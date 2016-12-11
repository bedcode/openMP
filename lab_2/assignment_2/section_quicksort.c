/********************************************************************************
 * Parallel code: section
 * Before execute the script type in a terminal:
 * OMP_STACKSIZE='1G'
 * export OMP_STACKSIZE
 * reference -> https://gcc.gnu.org/onlinedocs/libgomp/OMP_005fSTACKSIZE.html
********************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#define DEPTH_MAX 10
#define DEPTH_MIN 1
#define N 100000

int split(int a[], int lower, int upper);
void quicksort(int a[], int lower, int upper, int depth);
void quicksort_serial(int a[], int lower, int upper);


int main(int argc, char *argv[])
{
	int *arr;
	int i;
	double end, start = omp_get_wtime();

    arr = malloc(N*sizeof(int));
	for (i = 0; i < N; i++) {
		arr[i] = N - i;
	}

	quicksort(arr, 0, N - 1, DEPTH_MIN);

	end = omp_get_wtime();

	printf("\nArray after sorting:\n");

	for (i = 0; i <= N - 1; i++)
		printf("%d\t", arr[i]);

	printf("\nCalculation time: %f\n", end - start);
	free(arr);

	return 0;
}


void quicksort(int a[], int lower, int upper, int depth)
{
	if (depth > DEPTH_MAX) {
		quicksort_serial(a, lower, upper);
		return;
	}

	int i;
	if (upper > lower) {
		i = split(a, lower, upper);
		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
					quicksort(a, lower, i - 1, depth + 1);
				#pragma omp section
					quicksort(a, i + 1, upper, depth + 1);
			}
		}
	}
}


void quicksort_serial(int a[], int lower, int upper)
{
	int i;
	if (upper > lower)
	{
		i = split(a, lower, upper);
		quicksort_serial(a, lower, i - 1);
		quicksort_serial(a, i + 1, upper);
	}
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
