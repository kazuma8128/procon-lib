
// semigroup ver
template <typename T>
class disjoint_sparse_table {
	const int n;
	vector<vector<T>> t;
	T merge(const T& l, const T& r) const {
		return min(l, r); // operator
	}
public:
	disjoint_sparse_table(const vector<T>& b) : n(b.size()), t(1, b) {
		for (int i = 2, j = 1; i <= n; i <<= 1, j++) {
			t.emplace_back(n);
			for (int c = i; c < n; c += i << 1) {
				t[j][c - 1] = t[0][c - 1]; t[j][c] = t[0][c];
				for (int k = c + 1; k < c + i && k < n; ++k)
					t[j][k] = merge(t[j][k - 1], t[0][k]);
				for (int k = c - 2; k >= c - i; --k)
					t[j][k] = merge(t[0][k], t[j][k + 1]);
			}
		}
	}
	T find(int l, int r) const { // [l, r) : l < r
		assert(0 <= l && l < r && r <= n);
		if (l == --r) return t[0][l];
		int i = 31 - __builtin_clz(l ^ r);
		return merge(t[i][l], t[i][r]);
	}
	T operator[](int id) const {
		return t[0][id];
	}
};


// monoid ver
struct MIN {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(type l, type r) { return min(l, r); }
};

template <typename M>
class disjoint_sparse_table {
	using T = typename M::type;
	const int n;
	vector<vector<T>> t;
public:
	disjoint_sparse_table(const vector<T>& b) : n(b.size()), t(1, b) {
		for (int i = 2, j = 1; i <= n; i <<= 1, j++) {
			t.emplace_back(n);
			for (int c = i; c < n; c += i << 1) {
				t[j][c - 1] = t[0][c - 1]; t[j][c] = t[0][c];
				for (int k = c + 1; k < c + i && k < n; ++k)
					t[j][k] = M::op(t[j][k - 1], t[0][k]);
				for (int k = c - 2; k >= c - i; --k)
					t[j][k] = M::op(t[0][k], t[j][k + 1]);
			}
		}
	}
	T find(int l, int r) const { // [l, r)
		assert(0 <= l && l <= r && r <= n);
		if (l == r) return M::id();
		if (l == --r) return t[0][l];
		int i = 31 - __builtin_clz(l ^ r);
		return M::op(t[i][l], t[i][r]);
	}
	T operator[](int id) const {
		return t[0][id];
	}
};
