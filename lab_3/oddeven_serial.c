/************************************
 * serial code
 * it requires the size of the array
************************************/

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

void OddEvenSort(int *A, int N);

void OddEvenSort(int *A, int N)
{
    int exch = 1, start = 0, i;
    int temp;

    while (exch || start) {
        exch = 0;
        for (i = start; i < N-1; i += 2) {
            if (A[i] > A[i+1]) {
                temp = A[i];
                A[i] = A[i+1];
                A[i+1] = temp;
                exch = 1;
            }
        }
        if (start == 0)
            start = 1;
        else start = 0;
    }
}

int main(int argc, char *argv[]) {

    int *arr;
    int i, N;
    double end, start = omp_get_wtime();

    if (argc != 2) {
        printf("Insert the size of the array\n");
        return 1;
    }
    N = atoi(argv[1]);
    arr = malloc(N*sizeof(int));
    for (i = 0; i < N; i++) {
        arr[i] = N - i;
    }

    OddEvenSort(arr, N);
    end = omp_get_wtime();

    printf("Array after sorting\n");
    for (i = 0; i < N; i++)
        printf("%d\t", arr[i]);

    printf("\nCalculation time: %f", end - start);
    free(arr);
    return 0;
}
