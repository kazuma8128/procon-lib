
string doll(const string& S) {
	int N = S.size();
	if (N <= 1) return S;
	string res; res.push_back(S[0]);
	for (int i = 1; i < N; i++) {
		res.push_back('$');
		res.push_back(S[i]);
	}
	return res;
}

vector<int> manacher(const string& S) {
	int N = S.size();
	vector<int> R(N + 1);
	int i = 0, j = 0;
	while (i < N) {
		while (i - j >= 0 && i + j < N && S[i - j] == S[i + j]) j++;
		R[i] = j;
		int k = 1;
		while (i - k >= 0 && i + k < N && k + R[i - k] < j) {
			R[i + k] = R[i - k];
			k++;
		}
		i += k;
		j -= k;
	}
	return R;
}

class palindrome {
	int N;
	vector<int> A;
public:
	Palindrome(const string& S) : N(S.size()), A(manacher(doll(S))) {}
	bool is(int i, int j) {	// get whether S[i, j) is palindrome
		assert(0 <= i && i < j && j <= N);
		return A[i + j - 1] >= j - i;
	}
};
