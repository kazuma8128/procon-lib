
template <typename T>
class sparse_table {
	const int n;
	vector<vector<T>> t;
	T merge(const T& l, const T& r) const {
		return min(l, r);
	}
public:
	sparse_table(const vector<T>& b) : n(b.size()), t(1, b) {
		for (int i = 2, j = 1, p = 0; i <= n; i <<= 1, j++, p = 0) {
			t.emplace_back(n);
			for (int k = 0; k + i <= n; k++)
				t[j][p++] = merge(t[j - 1][k], t[j - 1][k + (i >> 1)]);
		}
	}
	T find(int l, int r) const { // [l, r)
		assert(0 <= l && l < r && r <= n);
		int i = 31 - __builtin_clz(r - l);
		return merge(t[i][l], t[i][r - (1 << i)]);
	}
	T operator[](int id) const {
		return t[0][id];
	}
};
