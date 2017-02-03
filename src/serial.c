#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NI 20        /* array sizes */
#define NJ 20
#define NSTEPS 500    /* number of time steps */

void init(int *old);
void evolve(int **old, int **new);
void update(int **old, int **new);
void show(int **new);

/* initialize elements of old to 0 or 1 */
void init(int *old) {
    int i, j;
    float x;
/*
    for(i = 1; i <= NI; i++) {
        for(j = 1; j <= NJ; j++){
            x = rand()/((float)RAND_MAX + 1);
            if(x < 0.5) {
                old[i*NI + j] = 0;
            } else {
                old[i*NI + j] = 1;
            }
        }
    }
*/
    j = 0;
    for (i = NJ + 3; i <= (NI+2)*(NJ+2) - NJ+3; i++) {
        if (j < NJ) {
            x = rand()/((float)RAND_MAX + 1);
            if(x < 0.5) {
                old[i] = 0;
            } else {
                old[i] = 1;
            }
            j++;
        } else {
            j = 0;
            i = i + 1;
        }
    }
}

void evolve(int **old, int **new) {
    int i, j, n, im, ip, jm, jp, nsum;
    /*  time steps */
    for(n = 0; n < NSTEPS; n++) {

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

                switch(nsum) {

                    case 3:
                        new[i][j] = 1;
                        break;

                    case 2:
                        new[i][j] = old[i][j];
                        break;

                    default:
                        new[i][j] = 0;
                }
            }
        }
    }
}

/* copy new state into old state */
void update(int **old, int **new) {
    int n, i, j;

    for(n = 0; n < NSTEPS; n++) {
        for(i = 1; i <= NI; i++) {
            for(j = 1; j <= NJ; j++) {
                old[i][j] = new[i][j];
            }
        }
    }
}

void show(int **new) {
    int i, j, isum = 0;

    for(i = 1; i <= NI; i++) {
        for(j = 1; j <= NJ; j++) {
            isum = isum + new[i][j];
        }
    }
}

int main(int argc, char *argv[]) {

    int ni, nj;
    int *old, *new;
    int i, j;

    /* allocate arrays */
    ni = NI + 2;  /* add 2 for left and right ghost cells */
    nj = NJ + 2;
    old = malloc(ni*nj*sizeof(int));
    new = malloc(ni*nj*sizeof(int));

    init(old);
    for(i = 0; i <= NI + 1; i++) {
        for(j = 0; j <= NJ +1 ; j++) {
            printf("%d", old[i*NI +j]);
        }
        printf("\n");
    }

/*
    ni = NI + 2;
    nj = NJ + 2;
    int **old, **new;
    int i, j;
    float x;
    old = malloc(ni*sizeof(int*));
    new = malloc(ni*sizeof(int*));

    for(i = 0; i < ni; i++) {
        old[i] = malloc(nj*sizeof(int));
        new[i] = malloc(nj*sizeof(int));
    }


    for(i = 1; i <= NI; i++) {
        for(j = 1; j <= NJ; j++){
            x = rand()/((float)RAND_MAX + 1);
            if(x < 0.5) {
                old[i][j] = 0;
            } else {
                old[i][j] = 1;
            }
        }
    }
    for(i = 0; i <= NI+ 1; i++) {
        for(j = 0; j <= NJ +1; j++){
            printf("%d", old[i][j]);
        }
        printf("\n");
    }*/

    return 0;
}
