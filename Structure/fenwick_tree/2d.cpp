
template <typename T>
class FenwickTree2 {
	const int h, w;
	vector<vector<T>> data;
public:
	FenwickTree2(int h_, int w_) : h(h_), w(w_), data(h, vector<T>(w)) {}
	T find(int pi, int pj) const {
		T res = 0;
		for (int i = pi; i >= 0; i = (i & (i + 1)) - 1) {
			for (int j = pj; j >= 0; j = (j & (j + 1)) - 1) {
				res += data[i][j];
			}
		}
		return res;
	}
	T find(int li, int lj, int ri, int rj) const {
		return find(ri, rj) - find(ri, lj - 1) - find(li - 1, rj) + find(li - 1, lj - 1);
	}
	void add(int pi, int pj, T val) {
		for (int i = pi; i < h; i |= i + 1) {
			for (int j = pj; j < w; j |= j + 1) {
				data[i][j] += val;
			}
		}
	}
	void update(int pi, int pj, T val) {
		add(pi, pj, val - find(pi, pj));
	}
};
