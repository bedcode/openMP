/*****************************************************************
 * parallel version with one parallel region - dynamic schedule
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NI 500      /* array sizes */
#define NJ 500
#define NSTEPS 2000  /* number of time steps */
#define BORDERI (2 + NI)
#define BORDERJ (2 + NJ)
#define CHUNCKSIZE 10

void init(int *old);
void evolve(int *old, int *new);
void update(int *old, int *new);
void show(int *array);

/* initialize elements of old to 0 or 1 */
void init(int *old) {
    int i, j;
    float x;

    #pragma omp for schedule(dynamic, CHUNCKSIZE) private(i, j, x)
    for(i = 0; i < BORDERI; i++) {
        for(j = 0; j < BORDERJ; j++){
            if (i==0 || j==0 || i==(BORDERI-1) || j==BORDERJ-1)
                old[i*(BORDERJ) + j] = 0;
            else {
                x = rand()/((float)RAND_MAX + 1);
                if(x < 0.5) {
                    old[i*(BORDERJ) + j] = 0;
                } else {
                    old[i*(BORDERJ) + j] = 1;
                }
            }
        }
    }
}

/* change from old state to new state */
void evolve(int *old, int *new) {
    int i, j, iu, id, jl, jr, nsum = 0;

    /* corner boundary conditions */
    old[0] = old[NI*BORDERJ + NJ];
    old[BORDERJ-1] = old[NI*BORDERJ + 1];
    old[(BORDERI-1)*(BORDERJ) + BORDERJ-1] = old[BORDERJ + 1];
    old[(BORDERI-1)*(BORDERJ)] = old[BORDERJ + NJ];

    /* left-right boundary conditions */
    #pragma omp for schedule(dynamic, CHUNCKSIZE) private(i)
    for(i = 1; i < BORDERI-1; i++) {
        old[i*(BORDERJ)] = old[i*(BORDERJ) + NJ];
        old[i*(BORDERJ) + BORDERJ-1] = old[i*(BORDERJ) + 1];
    }

    /* top-bottom boundary conditions */
    #pragma omp for schedule(dynamic, CHUNCKSIZE) private(j)
    for(j = 1; j < BORDERJ-1; j++){
        old[j] = old[NI*BORDERJ + j];
        old[(BORDERI-1)*(BORDERJ) + j] = old[BORDERJ + j];
    }

    #pragma omp barrier

    //show(old);
    #pragma omp for schedule(dynamic, CHUNCKSIZE) private(i, j)
    for(i = 1; i < BORDERI-1; i++) {
        for(j = 1; j < BORDERJ-1; j++){
            iu = i-1;   //up
            id = i+1;   //down
            jl = j-1;   //left
            jr = j+1;   //right

            nsum = old[iu * (BORDERJ) +jl] + old[iu * (BORDERJ) +j] + old[iu * (BORDERJ) +jr] + old[i * (BORDERJ) +jl] + old[i * (BORDERJ) +jr] + old[id * (BORDERJ) +jl] + old[id * (BORDERJ) +j] + old[id * (BORDERJ) +jr];
            //printf("i: %d j:%d sum :%d \n ", i , j, nsum);
            if (nsum == 3) {
                new[i*(BORDERJ) + j] = 1;
                nsum = 0;
            } else if (nsum == 2) {
                new[i*(BORDERJ) + j] = old[i*(BORDERJ) + j];
                nsum = 0;
            } else {
                new[i*(BORDERJ) + j] = 0;
                nsum = 0;
            }
        }
    }
}

/* copy new state into old state */
void update(int *old, int *new) {
    int i, j;
    #pragma omp for schedule(dynamic, CHUNCKSIZE) private(i, j)
    for(i = 1; i < BORDERI-1; i++) {
        for(j = 1; j < BORDERJ-1; j++){
            old[i*(BORDERJ) + j] = new[i*(BORDERJ) + j];
        }
    }
}

/* print a given array */
void show(int *array) {
    int i, j;

    for(i = 0; i < BORDERI; i++) {
        for(j = 0; j < BORDERJ; j++) {
            printf("%d", array[i*(BORDERJ) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    int *old, *new;
    int i, j;
    double end, start;

    /* allocate arrays */
    old = malloc(BORDERI * BORDERJ * sizeof(int));
    new = malloc(BORDERI * BORDERJ * sizeof(int));

    start = omp_get_wtime();

    #pragma omp parallel shared(old, new)
    {
        init(old);
        //show(old);

        for (int n = 0; n < NSTEPS; n++) {
            evolve(old, new);
            update(old, new);
        }
    }

    //show(old);
    end = omp_get_wtime();
    printf("Calculation time: %f\n", end - start);

    free(old);
    free(new);
    return 0;
}
