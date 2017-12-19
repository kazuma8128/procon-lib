
// range add and range sum
template <typename T>
class RARS {
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
	RARS(int n_) : n(size(n_)), id(0), data(n * 2, id), data2(n * 2, id) {}
	void add(int l, int r, T val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T getsum(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};

// lazy segment tree
struct RARMQ {
	using type = int;
	static type id1() { return INT_MAX; }
	static type id2() { return 0; }
	static type op1(const type& l, const type & r) { return min(l, r); }
	static type op2(const type& l, const type & r) { return l + r; }
};

struct RURS {
	using type = int;
	static type id1() { return 0; }
	static type id2() { return -1; }
	static type op1(const type& l, const type & r) { return l + r; }
	static type op2(const type& l, const type & r) { return r != -1 ? r : l; }
};

struct RURMQ {
	using type = int;
	static type id1() { return INT_MAX; }
	static type id2() { return -1; }
	static type op1(const type& l, const type & r) { return min(l, r); }
	static type op2(const type& l, const type & r) { return r != -1 ? r : l; }
};

struct RURMS {
	static const ll INF = 1e17;
	struct type {
		ll left, right, all, sum, size;
		type() : left(-INF), right(-INF), all(-INF), sum(0), size(0) {}
		type(ll val) : left(val), right(val), all(val), sum(val), size(1) {}
		type(ll l, ll r, ll a, ll s, ll si) : left(l), right(r), all(a), sum(s), size(si) {}
		bool operator==(const type& r) const {
			return left == r.left && right == r.right && all == r.all && sum == r.sum && size == r.size;
		}
	};
	static type id1() { return type(); }
	static type id2() { return type(-INF, -INF, -INF, -INF, -1); }
	static type op1(const type& l, const type & r) {
		return type(max(l.left, l.sum + r.left), max(r.right, r.sum + l.right), max({ l.all, r.all, l.right + r.left }), l.sum + r.sum, l.size + r.size);
	}
	static type op2(const type& l, const type & r) {
		if (r == id2()) return l;
		if (l.size < 0) return r;
		ll val = r.all;
		if (val > 0) {
			return type(val * l.size, val * l.size, val * l.size, val * l.size, l.size);
		}
		else {
			return type(val, val, val, val * l.size, l.size);
		}
	}
};

template <typename M>
class lSegmentTree {
	using T = typename M::type;
	const int n;
	vector<T> data, lazy;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	void push(int node) {
		if (lazy[node] == M::id2()) return;
		if (node < n) {
			lazy[node * 2] = M::op2(lazy[node * 2], lazy[node]);
			lazy[node * 2 + 1] = M::op2(lazy[node * 2 + 1], lazy[node]);
		}
		data[node] = M::op2(data[node], lazy[node]);
		lazy[node] = M::id2();
	}
	void suc(int l, int r, int node, int lb, int ub, T val) {
		if (ub <= l || r <= lb) return;
		push(node);
		if (l <= lb && ub <= r) {
			lazy[node] = val;
			return;
		}
		int c = (lb + ub) / 2;
		suc(l, r, node * 2, lb, c, val);
		suc(l, r, node * 2 + 1, c, ub, val);
		data[node] = M::op1(M::op2(data[node * 2], lazy[node * 2]), M::op2(data[node * 2 + 1], lazy[node * 2 + 1]));
	}
	T sub(int l, int r, int node, int lb, int ub) {
		if (ub <= l || r <= lb) return M::id1();
		push(node);
		if (l <= lb && ub <= r) {
			return data[node];
		}
		int c = (lb + ub) / 2;
		return M::op1(sub(l, r, node * 2, lb, c), sub(l, r, node * 2 + 1, c, ub));
	}
public:
	lSegmentTree(int n_) : n(size(n_)), data(n * 2, M::id1()), lazy(n * 2, M::id2()) {}
	void init(const vector<T>& data_) {
		for (int i = 0; i < (int)data_.size(); i++)
			data[i + n] = data_[i];
		for (int i = n - 1; i >= 0; i--)
			data[i] = M::op1(data[i * 2], data[i * 2 + 1]);
		fill(lazy.begin(), lazy.end(), M::id2());
	}
	void update(int l, int r, T val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T find(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};
