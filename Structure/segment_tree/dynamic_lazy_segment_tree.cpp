
// range add and range sum
using ll = long long;

template <typename T>
class node {
public:
	node<T> *l, *r;
	T val, add;
	node(T val_ = 0, node<T>* l_ = nullptr, node<T>* r_ = nullptr)
		: l(l_), r(r_), val(val_), add(0) {}
};

template <typename T>
class RARS {
	const ll n;
	node<T> *root;
	ll size(ll n) {
		ll res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	T sub(ll l, ll r, node<T>* n, ll lb, ll ub) {
		if (!n || ub <= l || r <= lb) return 0;
		if (l <= lb && ub <= r) return n->val + n->add * (ub - lb);
		return sub(l, r, n->l, lb, (lb + ub) / 2) + sub(l, r, n->r, (lb + ub) / 2, ub) + n->add * (min(r, ub) - max(l, lb));
	}
	node<T>* suc(ll l, ll r, node<T>* n, ll lb, ll ub, T val) {
		if (ub <= l || r <= lb) return n;
		if (!n) n = new node<T>;
		if (l <= lb && ub <= r) {
			n->add += val;
			return n;
		}
		n->val += val * (min(r, ub) - max(l, lb));
		n->l = suc(l, r, n->l, lb, (lb + ub) / 2, val);
		n->r = suc(l, r, n->r, (lb + ub) / 2, ub, val);
		return n;
	}
public:
	RARS(ll n_) : n(size(n_)), root(nullptr) {}
	void add(ll l, ll r, T val) {
		root = suc(l, r + 1, root, 0, n, val);
	}
	T getSum(ll l, ll r) {
		return sub(l, r + 1, root, 0, n);
	}
};

// generalized dynamic lazy segment tree
using ll = long long;

struct RMQ {
	using type = int;
	static type id1() { return INT_MAX; }
	static type id2() { return -1; }
	static type op1(const type& l, const type & r) { return min(l, r); }
	static type op2(const type& l, const type & r) { return r != -1 ? r : l; }
};

template <typename T>
class node {
public:
	T val, lazy;
	node<T> *l, *r;
	node() {}
	node(T val_, T lazy_, node<T>* l_ = nullptr, node<T>* r_ = nullptr) : val(val_), lazy(lazy_), l(l_), r(r_) {}
	void init(T val_, T lazy_, node<T>* l_ = nullptr, node<T>* r_ = nullptr) {
		val = val_;
		lazy = lazy_;
		l = l_;
		r = r_;
	}
};

template <typename M>
class DynamicLazySegmentTree {
	using T = typename M::type;
	const ll n;
	node<T> *root;
	vector<node<T>> pool;
	int it;
	ll size(ll n) {
		ll res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	node<T> *new_node() {
		pool[it].init(M::id1(), M::id2());
		return &pool[it++];
	}
	node<T> *push(node<T> *t, ll lb, ll ub) {
		if (t == nullptr || t->lazy == M::id2()) return t;
		if (lb + 1 < ub) {
			if (t->l == nullptr) t->l = new_node();
			if (t->r == nullptr) t->r = new_node();
			t->l->lazy = M::op2(t->l->lazy, t->lazy);
			t->r->lazy = M::op2(t->r->lazy, t->lazy);
		}
		t->val = M::op2(t->val, t->lazy);
		t->lazy = M::id2();
		return t;
	}
	node<T> *suc(ll l, ll r, node<T> *t, ll lb, ll ub, T val) {
		if (ub <= l || r <= lb) return t;
		if (t == nullptr) t = new_node();
		if (l <= lb && ub <= r) {
			t->lazy = M::op2(t->lazy, val);
			return t;
		}
		t = push(t, lb, ub);
		int c = (lb + ub) / 2;
		t->l = suc(l, r, t->l, lb, c, val);
		t->r = suc(l, r, t->r, c, ub, val);
		t->val = M::op1(t->l == nullptr ? M::id1() : M::op2(t->l->val, t->l->lazy), t->r == nullptr ? M::id1() : M::op2(t->r->val, t->r->lazy));
		return t;
	}
	T sub(ll l, ll r, node<T> *t, ll lb, ll ub) {
		if (t == nullptr || ub <= l || r <= lb) return M::id1();
		t = push(t, lb, ub);
		if (l <= lb && ub <= r) return t->val;
		int c = (lb + ub) / 2;
		return M::op1(sub(l, r, t->l, lb, c), sub(l, r, t->r, c, ub));
	}
public:
	DynamicLazySegmentTree(ll n_, int PMAX = 1e7)
		: n(size(n_)), root(nullptr), pool(PMAX), it(0) {}
	void update(ll l, ll r, T val) {
		root = suc(l, r + 1, root, 0, n, val);
	}
	T find(ll l, ll r) {
		return sub(l, r + 1, root, 0, n);
	}
};
