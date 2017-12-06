
struct RMQ {
	using type = int;
	static type id() { return 0; }
	static type op(const type& l, const type & r) { return max(l, r); }
};

template <typename M>
class StarrySkyTree {
	using T = typename M::type;
	const int n;
	vector<T> data, data2;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	T sub(int l, int r, int node, int lb, int ub) {
		if (ub <= l || r <= lb) return M::id();
		if (l <= lb && ub <= r) return data[node] + data2[node];
		return M::op(sub(l, r, node * 2, lb, (lb + ub) / 2), sub(l, r, node * 2 + 1, (lb + ub) / 2, ub)) + data2[node];
	}
	void suc(int l, int r, int node, int lb, int ub, T val) {
		if (ub <= l || r <= lb) return;
		if (l <= lb && ub <= r) {
			data2[node] += val;
			return;
		}
		int left = node * 2, right = node * 2 + 1;
		suc(l, r, left, lb, (lb + ub) / 2, val);
		suc(l, r, right, (lb + ub) / 2, ub, val);
		data[node] = M::op(data[left] + data2[left], data[right] + data2[right]);
	}
public:
	StarrySkyTree(int n_) : n(size(n_)), data(n * 2, 0), data2(n * 2, 0) {}
	void add(int l, int r, T val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T find(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};
