
template <typename T>
class SparseTable {
	int n;
	vector<vector<T>> t;
	T merge(const T& l, const T& r) {
		return min(l, r);
	}
public:
	SparseTable(const vector<T>& b) {
		n = b.size();
		t.push_back(b);
		for (int i = 2, j = 1; i <= n; i <<= 1, j++) {
			t.push_back(vector<T>());
			for (int k = 0; k + i <= n; k++) {
				t[j].push_back(merge(t[j - 1][k], t[j - 1][k + i / 2]));
			}
		}
	}
	T find(int l, int r) {
		r++;
		assert(0 <= l && l < r && r <= n);
		unsigned int w = r - l;
		int i = 31 - __builtin_clz(w);
		return merge(t[i][l], t[i][r - (1 << i)]);
	}
	T operator[](int id) {
		return t[0][id];
	}
};
