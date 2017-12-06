
// one dimension
template <typename T>
class imos {
	int n;
	vector<T> v;
public:
	imos(int n) : n(n), v(n + 1) {}
	T& val(int i) { assert(0 <= i && i < n); return v[i + 1]; }
	void build() { for (int i = 1; i <= n; i++) v[i] += v[i - 1]; }
	T sum(int l, int r) { assert(0 <= l && l <= r && r <= n); return v[r] - v[l]; }
};

// two dimensions
template <typename T>
class imos2 {
	int n, m;
	vector<vector<T>> v;
public:
	imos2(int n, int m) : n(n), m(m), v(n + 1, vector<T>(m + 1)) {}
	T& val(int i, int j) { assert(0 <= i && i < n && 0 <= j && j < m); return v[i + 1][j + 1]; }
	void build() {
		for (int i = 1; i <= n; i++) for (int j = 0; j <= m; j++) v[i][j] += v[i - 1][j];
		for (int i = 0; i <= n; i++) for (int j = 1; j <= m; j++) v[i][j] += v[i][j - 1];
	}
	T sum(int li, int lj, int ri, int rj) {
		assert(0 <= li && li <= ri && ri <= n);
		assert(0 <= lj && lj <= rj && rj <= m);
		return v[ri][rj] - v[ri][lj] - v[li][rj] + v[li][lj];
	}
};

// three dimensions
template <typename T>
class imos3 {
	int n, m, l;
	vector<vector<vector<T>>> v;
public:
	imos3(int n, int m, int l) : n(n), m(m), l(l), v(n + 1, vector<vector<T>>(m + 1, vector<T>(l + 1))) {}
	T& val(int i, int j, int k) { assert(0 <= i && i < n && 0 <= j && j < m && 0 <= k && k < l); return v[i + 1][j + 1][k + 1]; }
	void build() {
		for (int i = 1; i <= n; i++) for (int j = 0; j <= m; j++)  for (int k = 0; k <= l; k++) v[i][j][k] += v[i - 1][j][k];
		for (int i = 0; i <= n; i++) for (int j = 1; j <= m; j++)  for (int k = 0; k <= l; k++) v[i][j][k] += v[i][j - 1][k];
		for (int i = 0; i <= n; i++) for (int j = 0; j <= m; j++)  for (int k = 1; k <= l; k++) v[i][j][k] += v[i][j][k - 1];
	}
	T sum(int li, int lj, int lk, int ri, int rj, int rk) {
		assert(0 <= li && li <= ri && ri <= n);
		assert(0 <= lj && lj <= rj && rj <= m);
		assert(0 <= lk && lk <= rk && rk <= l);
		return v[ri][rj][rk] - v[li][rj][rk] - v[ri][lj][rk] - v[ri][rj][lk]
			+ v[ri][lj][lk] + v[li][rj][lk] + v[li][lj][rk] - v[li][lj][lk];
	}
};
