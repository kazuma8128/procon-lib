
vector<int> morris_pratt(const string& S) {
	int N = S.size();
	vector<int> A(N + 1);
	A[0] = -1;
	for (int i = 0, j = -1; i < N; i++) {
		while (j >= 0 && S[i] != S[j]) {
			j = A[j];
		}
		j++;
		A[i + 1] = j;
	}
	return A;
}

vector<int> knuth_morris_pratt(const string& S) {
	int N = S.size();
	vector<int> A(N + 1);
	A[0] = -1;
	for (int i = 0, j = -1; i < N; i++) {
		while (j >= 0 && S[i] != S[j]) {
			j = A[j];
		}
		j++;
		A[i + 1] = (i + 1 == N ? '*' : S[i + 1]) == (j == N ? '*' : S[j]) ? A[j] : j;
	}
	return A;
}

vector<int> string_search(const string& S, const string& T) {
	auto A = knuth_morris_pratt(T);
	int N = T.size();
	vector<int> res;
	for (int i = 0, j = 0; i < (int)S.size(); i++) {
		while (j >= 0 && S[i] != T[j]) {
			j = A[j];
		}
		j++;
		if (j == N) {
			res.push_back(i - N + 1);
			j = A[j];
		}
	}
	return res;
}
