
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

// monoids
struct RU {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return INT_MAX; }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return min(l, r); }
	static t1 op2(const t1& l, const t2& r) { return r == id2() ? l : r; }
	static t2 op3(const t2& l, const t2& r) { return r == id2() ? l : r; }
};

struct RA {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return 0; }
	static t2 id2() { return 0; }
	static t1 op1(const t1& l, const t1& r) { return l + r; }
	static t1 op2(const t1& l, const t2& r) { return l + r; }
	static t2 op3(const t2& l, const t2& r) { return l + r; }
};

struct RURM {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return INT_MAX; }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return min(l, r); }
	static t1 op2(const t1& l, const t2& r) { return r != id2() ? r : l; }
	static t2 op3(const t2& l, const t2& r) { return r != id2() ? r : l; }
};

struct RURS {
	using ll = long long;
	struct t1 {
		ll val, size;
		t1(ll v, ll s = 1) : val(v), size(s) {}
	};
	using t2 = ll;
	static t1 id1() { return t1(0, 0); }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return t1(l.val + r.val, l.size + r.size); }
	static t1 op2(const t1& l, const t2& r) { return t1(l.size * r, l.size); }
	static t2 op3(const t2& l, const t2& r) { return r != id2() ? r : l; }
};

struct RARS {
	using ll = long long;
	struct t1 {
		ll val, size;
		t1(ll v, ll s = 1) : val(v), size(s) {}
	};
	using t2 = ll;
	static t1 id1() { return t1(0, 0); }
	static t2 id2() { return 0; }
	static t1 op1(const t1& l, const t1& r) { return t1(l.val + r.val, l.size + r.size); }
	static t1 op2(const t1& l, const t2& r) { return t1(l.val + l.size * r, l.size); }
	static t2 op3(const t2& l, const t2& r) { return l + r; }
};

struct RURMS {
	using ll = long long;
	static const ll INF = 1e17;
	struct t1 {
		ll left, right, all, sum, size;
		t1() : left(-INF), right(-INF), all(-INF), sum(0), size(0) {}
		t1(ll val) : left(val), right(val), all(val), sum(val), size(1) {}
		t1(ll l, ll r, ll a, ll s, ll si) : left(l), right(r), all(a), sum(s), size(si) {}
		bool operator==(const t1& r) const {
			return left == r.left && right == r.right && all == r.all && sum == r.sum && size == r.size;
		}
	};
	using t2 = ll;
	static t1 id1() { return t1(); }
	static t2 id2() { return -INF; }
	static t1 op1(const t1& l, const t1 & r) {
		return t1(max(l.left, l.sum + r.left), max(r.right, r.sum + l.right), max({ l.all, r.all, l.right + r.left }), l.sum + r.sum, l.size + r.size);
	}
	static t1 op2(const t1& l, const t2 & r) {
		if (r == id2()) return l;
		if (r > 0) return t1(r * l.size, r * l.size, r * l.size, r * l.size, l.size);
		return t1(r, r, r, r * l.size, l.size);
	}
	static t2 op3(const t2& l, const t2 & r) {
		return r == id2() ? l : r;
	}
};

// lazy segment tree
template <typename M>
class LazySegmentTree {
	using T1 = typename M::t1;
	using T2 = typename M::t2;
	const int n;
	vector<T1> data;
	vector<T2> lazy;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	void push(int node) {
		if (lazy[node] == M::id2()) return;
		if (node < n) {
			lazy[node * 2] = M::op3(lazy[node * 2], lazy[node]);
			lazy[node * 2 + 1] = M::op3(lazy[node * 2 + 1], lazy[node]);
		}
		data[node] = M::op2(data[node], lazy[node]);
		lazy[node] = M::id2();
	}
	void suc(int l, int r, int node, int lb, int ub, T2 val) {
		if (ub <= l || r <= lb) return;
		if (l <= lb && ub <= r) {
			lazy[node] = M::op3(lazy[node], val);
			return;
		}
		push(node);
		int c = (lb + ub) / 2;
		suc(l, r, node * 2, lb, c, val);
		suc(l, r, node * 2 + 1, c, ub, val);
		data[node] = M::op1(M::op2(data[node * 2], lazy[node * 2]), M::op2(data[node * 2 + 1], lazy[node * 2 + 1]));
	}
	T1 sub(int l, int r, int node, int lb, int ub) {
		if (ub <= l || r <= lb) return M::id1();
		if (l <= lb && ub <= r) return M::op2(data[node], lazy[node]);
		push(node);
		int c = (lb + ub) / 2;
		return M::op1(sub(l, r, node * 2, lb, c), sub(l, r, node * 2 + 1, c, ub));
	}
public:
	LazySegmentTree(int n_) : n(size(n_)), data(n * 2, M::id1()), lazy(n * 2, M::id2()) {}
	LazySegmentTree(int n_, T1 v1, T2 v2) : n(size(n_)), data(n * 2, v1), lazy(n * 2, v2) {}
	LazySegmentTree(const vector<T1>& data_) : n(size(data_.size())), data(n * 2, M::id1()), lazy(n * 2, M::id2()) {
		init(data_);
	}
	void init() {
		for (int i = n - 1; i >= 1; i--) data[i] = M::op1(data[i * 2], data[i * 2 + 1]);
	}
	void init(const vector<T1>& data_) {
		for (int i = 0; i < (int)data_.size(); i++) data[i + n] = data_[i];
		init();
	}
	void update(int l, int r, T2 val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T1 find(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};
