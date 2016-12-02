#include "stdio.h"
#define DEPTHMAX 4

int split(int*, int, int ) ;
void quicksort(int a[], int lower, int upper, int depth);
void quicksort_serial(int a[], int lower, int upper);


int main(int argc, char *argv[])
{
	int arr[10] = {11, 2, 9, 13, 57, 25, 17, 1, 90, 3};
	int i;

	quicksort(arr, 0, 9, 0);

	printf("\nArray after sorting:\n");

	for (i = 0 ; i <= 9 ; i++)
		printf("%d\t", arr[i]);

	return 0;
}


void quicksort(int a[], int lower, int upper, int depth)
{
	if (depth > DEPTHMAX) {
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
					quicksort (a, lower, i - 1 , depth + 1);
				#pragma omp section
					quicksort (a, i + 1, upper , depth + 1);
			}
		}
	}
}


void quicksort_serial ( int a[ ], int lower, int upper )
{
	int i, section;
	if ( upper > lower )
	{

		i = split ( a, lower, upper ) ;

		quicksort_serial( a, lower, i - 1 ) ;

		quicksort_serial( a, i + 1, upper ) ;

	 }
}


int split ( int a[ ], int lower, int upper )
{
	int i, p, q, t ;

	p = lower + 1 ;
	q = upper ;
	i = a[lower] ;

	while ( q >= p )
	{
		while ( a[p] < i )
			p++ ;

		while ( a[q] > i )
			q-- ;

		if ( q > p )
		{
			t = a[p] ;
			a[p] = a[q] ;
			a[q] = t ;
		}
	}

	t = a[lower] ;
	a[lower] = a[q] ;
	a[q] = t ;

	return q ;
}
