#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NI 20        /* array sizes */
#define NJ 20
#define NSTEPS 500    /* number of time steps */

void init(int *old);
void evolve(int *old, int *new);
void update(int *old, int *new);
void show(int *array);

/* initialize elements of old to 0 or 1 */
void init(int *old) {
    int i, j;
    float x;

    for(i = 0; i < NI+2; i++) {
        for(j = 0; j < NJ+2; j++){
            if (i==0 || j==0 || i==NI+1 || j==NJ+1)
                old[i*(NJ+1)+j]=0;
            else {
                x = rand()/((float)RAND_MAX + 1);
                if(x < 0.5) {
                    old[i*(NJ+1)+j] = 0;
                } else {
                    old[i*(NJ+1)+j] = 1;
                }
            }
        }
    }
}

/* change from old state to new state */
void evolve(int *old, int *new) {
    int i, j, im, ip, jm, jp, nsum;

    /* corner boundary conditions */
    old[0][0] = old[NI][NJ];
    old[0][NJ+1] = old[NI][1];
    old[NI+1][NJ+1] = old[1][1];
    old[NI+1][0] = old[1][NJ];

    /* left-right boundary conditions */
    for(i = 1; i <= NI; i++){
        old[i][0] = old[i][NJ];
        old[i][NJ+1] = old[i][1];
    }

    /* top-bottom boundary conditions */
    for(j = 1; j <= NJ; j++){
        old[0][j] = old[NI][j];
        old[NI+1][j] = old[1][j];
    }

    for(i = 1; i <= NI; i++) {
        for(j = 1; j<=NJ; j++) {
            im = i-1;
            ip = i+1;
            jm = j-1;
            jp = j+1;

            nsum = old[im][jp] + old[i][jp] + old[ip][jp] + old[im][j] + old[ip][j] + old[im][jm] + old[i][jm] + old[ip][jm];

            if (nsum == 3) {
                new[i*(NJ+1) + j] = 1;
            } else if (nsum == 2) {
                new[i*(NJ+1) + j] = old[i*(NJ+1) + j];
            } else {
                new[i*(NJ+1) + j] = 0;
            }
        }
    }
}

/* copy new state into old state */
void update(int *old, int *new) {
    int i, j;

    for(i = 0; i < NI+2; i++) {
        for(j = 0; j < NJ+2; j++){
            if (i==0 || j==0 || i==NI+1 || j==NJ+1)
                continue;
            else
                old[i*(NJ+1) + j] = new[i*(NJ+1) + j];
        }
    }
}

/* print a given array */
void show(int *array) {
    int i, j;

    for(i = 0; i <= NI + 1; i++) {
        for(j = 0; j <= NJ + 1; j++) {
            printf("%d", array[i*(NJ+1) + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    int ni, nj;
    int *old, *new;
    int i, j, n;

    /* allocate arrays */
    ni = NI + 2;  /* add 2 for left and right ghost cells */
    nj = NJ + 2;
    old = malloc(ni*nj*sizeof(int));
    new = malloc(ni*nj*sizeof(int));

    init(old);

    /*for (n = 0, n < NSTEPS; n++) {
        evolve(old, new);
        update(old, new);
    }*/

    show(old);

    return 0;
}
