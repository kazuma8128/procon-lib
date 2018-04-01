
template <typename T>
class range_add_range_sum {
	const int n;
	const T id;
	vector<T> data, data2;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	T sub(int l, int r, int node, int lb, int ub) {
		if (ub <= l || r <= lb) return id;
		if (l <= lb && ub <= r) return data[node] + data2[node] * (ub - lb);
		return data2[node] * (min(r, ub) - max(l, lb))
			+ sub(l, r, node * 2, lb, (lb + ub) / 2)
			+ sub(l, r, node * 2 + 1, (lb + ub) / 2, ub);
	}
	void suc(int l, int r, int node, int lb, int ub, T val) {
		if (ub <= l || r <= lb) return;
		if (l <= lb && ub <= r) {
			data2[node] += val;
			return;
		}
		data[node] += val * (min(r, ub) - max(l, lb));
		suc(l, r, node * 2, lb, (lb + ub) / 2, val);
		suc(l, r, node * 2 + 1, (lb + ub) / 2, ub, val);
	}
public:
	range_add_range_sum(int n_) : n(size(n_)), id(0), data(n * 2, id), data2(n * 2, id) {}
	void add(int l, int r, T val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T getsum(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};
