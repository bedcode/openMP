void OddEvenSort(int *A, int N)
{
	int exch = 1, start = 0, i;

#pragma omp parallel
	{
		int temp;
		while (exch || start) {
			exch = 0;
#pragma omp for
			for (i = start; i < N; i += 2){
				if (A[i] > A[i+1]) {
					temp = A[i]; A[i] = A[i+1]; A[i+1] = temp;
					exch = 1;
				}
			}
#pragma omp single
			if (start == 0) start = 1;
			else start = 0;
		}
	}
}

