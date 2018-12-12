
template <typename T>
class fenwick_tree_3d {
	const int a, b, c;
	vector<vector<vector<T>>> data;
public:
	fenwick_tree_3d(int a_, int b_, int c_)
		: a(a_), b(b_), c(c_), data(a, vector<vector<T>>(b, vector<T>(c))) {}
	T find(int pi, int pj, int pk) const { // [0, pi] & [0, pj] & [0, pk]
		T res = 0;
		for (int i = pi; i >= 0; i = (i & (i + 1)) - 1) {
			for (int j = pj; j >= 0; j = (j & (j + 1)) - 1) {
				for (int k = pk; k >= 0; k = (k & (k + 1)) - 1) {
					res += data[i][j][k];
				}
			}
		}
		return res;
	}
	T find(int li, int lj, int lk, int ri, int rj, int rk) const { // [li, ri] & [lj, rj] & [lk, rk]
		--li, --lj, --lk;
		return find(ri, rj, rk)
			- find(ri, rj, lk) - find(ri, lj, rk) - find(li, rj, rk)
			+ find(ri, lj, lk) + find(li, rj, lk) + find(li, lj, rk)
			- find(li, lj, lk);
	}
	void add(int pi, int pj, int pk, T val) {
		for (int i = pi; i < a; i |= i + 1) {
			for (int j = pj; j < b; j |= j + 1) {
				for (int k = pk; k < c; k |= k + 1) {
					data[i][j][k] += val;
				}
			}
		}
	}
	void update(int pi, int pj, int pk, T val) {
		add(pi, pj, pk, val - find(pi, pj, pk, pi, pj, pk));
	}
};
