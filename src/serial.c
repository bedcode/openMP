#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NI 4      /* array sizes */
#define NJ 4
#define NSTEPS 1    /* number of time steps */
#define BOUND 2
#define BORDERI (BOUND + NI)
#define BORDERJ (BOUND + NJ)
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
                old[i*(NJ+2)+j]=0;
            else {
                x = rand()/((float)RAND_MAX + 1);
                if(x < 0.5) {
                    old[i*(NJ+2)+j] = 0;
                } else {
                    old[i*(NJ+2)+j] = 1;
                }
            }
        }
    }
    
}

/* change from old state to new state */
void evolve(int *old, int *new) {
    int i, j, iu, id, jl, jr, nsum= 0;
    
    /* corner boundary conditions */
    old[0] = old[(NI)*(NJ +2) + (NJ)];
    old[NJ+1] = old[NI*(NJ +2) + 1];
    old[(NI+1)*(NJ+2) + NJ +1] = old[(NJ +2) + 1];
    old[(NI+1)*(NJ + 2)] = old[1* (NJ +2)+ NJ];

    /* left-right boundary conditions */
    for(i = 1; i < NI+1; i++) {
        
            old[i*(NJ +2)] = old[i*(NJ+2)+ NJ];
            old[i*(NJ +2)+NJ+1] = old[i*(NJ +2) + 1];
    	
    }

    /* top-bottom boundary conditions */
    for(j = 1; j <NJ +1; j++){
    	
        old[j] = old[NI*(NJ +2) + j];
        old[(NI+1)*(NJ +2) + j] = old[((NJ +2) +j)];
    }
    show(old);
    for(i = 1; i < NI+1; i++) {
    	
        for(j = 1; j < NJ+1; j++){
        
            iu = i-1;		//up
            id = i+1;		//down
            jl = j-1;		//left
            jr = j+1;		//right

            nsum = old[iu * (NJ +2) +jl] + old[iu * (NJ +2) +j]	+ old[iu * (NJ +2) +jr] + old[i * (NJ +2) +jl] + old[i * (NJ +2) +jr] + old[id * (NJ +2) +jl] + old[id * (NJ +2) +j] + old[id * (NJ +2) +jr];
            printf("i: %d j:%d sum :%d \n ", i , j, nsum );
            
            if (nsum == 3) {
                new[i*(NJ+2) + j] = 1;
                nsum = 0;
            } else if (nsum == 2) {
                new[i*(NJ+2) + j] = old[i*(NJ+2) + j];
                nsum = 0;
            } else {
                new[i*(NJ+2) + j] = 0;
                nsum = 0;
            }
        }
    }
}

/* copy new state into old state */
void update(int *old, int *new) {
    int i, j;

    for(i = 1; i < NI+1; i++) {
        for(j = 1; j < NJ+1; j++){
            
                old[i*(NJ+2) + j] = new[i*(NJ+2) + j];
        }
    }
}

/* print a given array */
void show(int *array) {
    int i, j;

    for(i = 0; i <= NI + 1; i++) {
        for(j = 0; j <= NJ + 1; j++) {
            printf("%d", array[i*(NJ+2) + j]);
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
    show(old);
    printf("\n\n");
    for (n = 0; n < NSTEPS; n++) {
        evolve(old, new);
        update(old, new);
    }

    show(old);

    return 0;
}
