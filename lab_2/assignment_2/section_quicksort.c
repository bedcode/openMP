/****************************
 * Parallel code: section
****************************/
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "omp.h"
#define DEPTH_MAX 4
#define DEPTH_MIN 1

int split(int a[], int lower, int upper);
void quicksort(int a[], int lower, int upper, int depth);
void quicksort_serial(int a[], int lower, int upper);


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

	quicksort(arr, 0, 99, DEPTH_MIN);

	end = omp_get_wtime();

	printf("Calculation time: %f\n", (end - start));
	printf("\nArray after sorting:\n");

	for (i = 0 ; i <= 99 ; i++)
		printf("%d\t", arr[i]);

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
		i = split (a, lower, upper);
		#pragma omp parallel
		{
			#pragma omp sections
			{
				#pragma omp section
					quicksort (a, lower, i - 1, depth + 1);
				#pragma omp section
					quicksort (a, i + 1, upper, depth + 1);
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
