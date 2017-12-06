
vector<int> Z_algo(const string& S) {
	int N = S.size();
	vector<int> A(N);
	A[0] = N;
	int i = 1, j = 0;
	while (i < N) {
		while (i + j < N && S[j] == S[i + j]) j++;
		A[i] = j;
		if (j == 0) {
			i++;
			continue;
		}
		int k = 1;
		while (i + k < N && k + A[k] < j) {
			A[i + k] = A[k];
			k++;
		}
		i += k;
		j -= k;
	}
	return A;
}
