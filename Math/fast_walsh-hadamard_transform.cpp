
// ƒ°(i ^ j == k) a_i * b_j -> v_k
template <typename T>
void fwt(vector<T>& f) {
	int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = f[j], y = f[j | i];
				f[j] = x + y, f[j | i] = x - y;
			}
		}
	}
}

template <typename T>
void ifwt(vector<T>& f) {
	int n = f.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = f[j], y = f[j | i];
				f[j] = (x + y) / 2, f[j | i] = (x - y) / 2;
			}
		}
	}
}

template <typename T>
vector<T> convolution(vector<T> a, vector<T> b) {
	int n = 1;
	while (n < (int)a.size() || n < (int)b.size()) n <<= 1;
	a.resize(n);
	b.resize(n);
	fwt(a), fwt(b);
	for (int i = 0; i < n; i++) {
		a[i] *= b[i];
	}
	ifwt(a);
	return a;
}
