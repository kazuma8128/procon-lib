
template <typename T>
class FenwickTree {
	const int n;
	vector<T> data;
public:
	FenwickTree(int n_) : n(n_), data(n) {}
	T find(int p) const {
		T res = 0;
		while (p >= 0) {
			res += data[p];
			p = (p & (p + 1)) - 1;
		}
		return res;
	}
	void add(int p, T val) {
		while (p < n) {
			data[p] += val;
			p |= p + 1;
		}
	}
};

// [l, r)
template <typename T>
class RARS {
	const int n;
	FenwickTree<T> bit0, bit1;
public:
	RARS(int n_) : n(n_), bit0(n), bit1(n) {}
	T find(int p) const {
		return bit1.find(p) * (p + 1) + bit0.find(p);
	}
	T find(int l, int r) const {
		return find(r - 1) - find(l - 1);
	}
	void add(int l, int r, T val) {
		bit0.add(l, -val * l);
		bit1.add(l, val);
		bit0.add(r, val * r);
		bit1.add(r, -val);
	}
};
