void OddEvenSort(int *A, int N)
{
	int exch = 1, start = 0, i;
	int temp;

	while (exch || start) {
		exch = 0;
		for (i = start; i < N-1; i += 2) {
			if (A[i] > A[i+1]) {
				temp = A[i]; A[i] = A[i+1]; A[i+1] = temp;
				exch = 1;
			}
		}
		if (start == 0) start = 1;
		else start = 0;
	}
}

