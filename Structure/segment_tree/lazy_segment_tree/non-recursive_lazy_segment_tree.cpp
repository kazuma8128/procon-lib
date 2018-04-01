
template <typename M>
class lazy_segment_tree {
	using T1 = typename M::t1;
	using T2 = typename M::t2;
	const int h, n;
	vector<T1> data;
	vector<T2> lazy;
	void push(int node) {
		if (lazy[node] == M::id2()) return;
		if (node < n) {
			lazy[node * 2] = M::op3(lazy[node * 2], lazy[node]);
			lazy[node * 2 + 1] = M::op3(lazy[node * 2 + 1], lazy[node]);
		}
		data[node] = M::op2(data[node], lazy[node]);
		lazy[node] = M::id2();
	}
	void update(int node) {
		data[node] = M::op1(M::op2(data[node * 2], lazy[node * 2]), M::op2(data[node * 2 + 1], lazy[node * 2 + 1]));
	}
public:
	lazy_segment_tree(int n_)
		: h(ceil(log2(n_))), n(1 << h), data(n * 2, M::id1()), lazy(n * 2, M::id2()) {}
	lazy_segment_tree(int n_, T1 v1)
		: h(ceil(log2(n_))), n(1 << h), data(n * 2, v1), lazy(n * 2, M::id2()) {}
	lazy_segment_tree(const vector<T1>& data_)
		: h(ceil(log2(data_.size()))), n(1 << h), data(n * 2, M::id1()), lazy(n * 2, M::id2()) {
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
		l += n, r += n;
		for (int i = h; i > 0; i--) push(l >> i), push(r >> i);
		int tl = l, tr = r;
		r++;
		while (l < r) {
			if (l & 1) lazy[l] = M::op3(lazy[l], val), l++;
			if (r & 1) r--, lazy[r] = M::op3(lazy[r], val);
			l >>= 1; r >>= 1;
		}
		while (tl >>= 1, tr >>= 1, tl) {
			if (lazy[tl] == M::id2()) update(tl);
			if (lazy[tr] == M::id2()) update(tr);
		}
	}
	T1 find(int l, int r) {
		l += n, r += n;
		for (int i = h; i > 0; i--) push(l >> i), push(r >> i);
		r++;
		T1 res1 = M::id1(), res2 = M::id1();
		while (l < r) {
			if (l & 1) res1 = M::op1(res1, M::op2(data[l], lazy[l])), l++;
			if (r & 1) r--, res2 = M::op1(M::op2(data[r], lazy[r]), res2);
			l >>= 1; r >>= 1;
		}
		return M::op1(res1, res2);
	}
};
