
struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

struct RSQ {
	using type = int;
	static type id() { return 0; }
	static type op(const type& l, const type & r) { return l + r; }
};

struct RMSQ {
	struct type {
		ll left, right, all, sum;
		type() : left(-INF), right(-INF), all(-INF), sum(0) {}
		type(ll val) : left(val), right(val), all(val), sum(val) {}
		type(ll l, ll r, ll a, ll s) : left(l), right(r), all(a), sum(s) {}
	};
	static type id() { return type(); }
	static type op(const type& l, const type & r) {
		return type(max(l.left, l.sum + r.left), max(r.right, r.sum + l.right), max({ l.all, r.all, l.right + r.left }), l.sum + r.sum);
	}
};

// segment tree
template <typename M>
class SegmentTree {
	using T = typename M::type;
	const int n;
	vector<T> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
public:
	SegmentTree(int n_) : n(size(n_)), data(n * 2, M::id()) {}
	void init(const vector<T>& data_) {
		for (int i = 0; i < (int)data_.size(); i++)
			data[i + n] = data_[i];
		for (int i = n - 1; i >= 0; i--)
			data[i] = M::op(data[i * 2], data[i * 2 + 1]);
	}
	void update(int p, T val) {
		p += n;
		data[p] = val;
		while (p >>= 1) data[p] = M::op(data[p * 2], data[p * 2 + 1]);
	}
	void add(int p, T val) {
		p += n;
		data[p] += val;
		while (p >>= 1) data[p] = M::op(data[p * 2], data[p * 2 + 1]);
	}
	T find(int l, int r) {
		l += n; r += n + 1;
		T res1 = M::id(), res2 = M::id();
		while (l < r) {
			if (l & 1) res1 = M::op(res1, data[l++]);
			if (r & 1) res2 = M::op(data[--r], res2);
			l >>= 1; r >>= 1;
		}
		return M::op(res1, res2);
	}
};
