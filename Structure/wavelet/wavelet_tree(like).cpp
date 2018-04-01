
template <typename T>
class wavelet_tree {
	int lo, N;
	vector<vector<int>> mat;
	vector<T> data;
	int log2(int n) {
		int res = 0;
		while ((1 << res) < n) res++;
		return res;
	}
	int sum(int l, int r, int d) {
		if (l >= r) return 0;
		return mat[d][r - 1] - (l ? mat[d][l - 1] : 0);
	}
	T sub(int l, int r, int k, int d, int lb, int ub) {
		if (d == lo) return data[lb];
		int s = sum(l, r, d);
		if (k < s) {
			return sub(lb + sum(lb, l, d), lb + sum(lb, r, d), k, d + 1, lb, (lb + ub) / 2);
		}
		else {
			return sub(l + sum(l, ub, d), r + sum(r, ub, d), k - s, d + 1, (lb + ub) / 2, ub);
		}
	}
public:
	wavelet_tree(const vector<T>& v, T inf) : lo(log2(v.size())), N(1 << lo), mat(lo, vector<int>(N)), data(v) {
		int sz = v.size();
		vector<pair<T, int>> state(N);
		for (int i = 0; i < N; i++) {
			state[i] = { (i < sz ? v[i] : inf), i };
		}
		sort(state.begin(), state.end());
		for (int i = lo - 1, seg = 2; i >= 0; i--, seg <<= 1) {
			auto prev = state;
			for (int j = 0; j < N; j += seg) {
				for (int k = j, l = j, r = j + seg / 2; k < j + seg; k++) {
					if (r == j + seg) {
						state[k] = prev[l++];
						mat[i][k]++;
					}
					else if (l == j + seg / 2) {
						state[k] = prev[r++];
					}
					else if (prev[l].second < prev[r].second) {
						state[k] = prev[l++];
						mat[i][k]++;
					}
					else {
						state[k] = prev[r++];
					}
				}
			}
			for (int j = 1; j < N; j++) {
				mat[i][j] += mat[i][j - 1];
			}
		}
		sort(data.begin(), data.end());
	}
	T select(int l, int r, int k) {
		return sub(l, r + 1, k, 0, 0, N);
	}
	void print() {
		cout << "lo N = " << lo << ' ' << N << endl;
		for (int i = 0; i < lo; i++) {
			for (int j = 0; j < N; j++) {
				cout << mat[i][j] << " \n"[j + 1 == N];
			}
		}
	}
};
