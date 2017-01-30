/************************************
 * parallel code - final version
 * it requires the size of the array
************************************/

#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

void OddEvenSort(int *A, int N);

void OddEvenSort(int *A, int N)
{
    int exch0, exch1 = 1, trips = 0, i;

    while (exch1) {
        exch0 = 0;
        exch1 = 0;

        #pragma omp parallel
        {
            int temp;
            #pragma omp for
                for (i = 0; i < N-1; i += 2) {
                    if (A[i] > A[i+1]) {
                        temp = A[i];
                        A[i] = A[i+1];
                        A[i+1] = temp;
                        exch0 = 1;
                    }
                }

            if (exch0 || !trips) {
                #pragma omp for
                    for (i = 1; i < N-1; i += 2) {
                        if (A[i] > A[i+1]) {
                            temp = A[i];
                            A[i] = A[i+1];
                            A[i+1] = temp;
                            exch1 = 1;
                        }
                    }
            } // if exch0
        } // end parallel
        trips = 1;
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
