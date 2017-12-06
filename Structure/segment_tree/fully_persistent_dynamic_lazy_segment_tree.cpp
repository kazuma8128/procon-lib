
// range add and range sum
using ll = long long;
const int PMAX = 10000000;

template <typename T>
struct node {
	T val, add;
	node *l, *r;
	node() {}
	node(T val, T add, node *left = nullptr, node *right = nullptr) : val(val), add(add), l(left), r(right) {}
	void init(T v, T a, node *left, node *right) {
		val = v;
		add = a;
		l = left;
		r = right;
	}
};

node<ll> pool[PMAX];

template <typename T>
class SaikyoSegmentTree {
	const ll n;
	const T id;
	int it;
	vector<node<T>*> root;
	T getval(node<T> *x) {
		return x == nullptr ? id : x->val;
	}
	T getadd(node<T> *x) {
		return x == nullptr ? id : x->add;
	}
	node<T>* getl(node<T> *x) {
		return x == nullptr ? nullptr : x->l;
	}
	node<T>* getr(node<T> *x) {
		return x == nullptr ? nullptr : x->r;
	}
	ll size(ll n) {
		assert(n < (1ll << 62ll));
		ll res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	node<T> *new_node(T val, T add, node<T> *l = nullptr, node<T> *r = nullptr) {
		pool[it].init(val, add, l, r);
		return &pool[it++];
	}
	node<T> *fix(node<T> *x, ll l, ll r, T val, ll lb, ll ub) {
		if (r <= lb || ub <= l) return x;
		if (l <= lb && ub <= r) return new_node(getval(x), getadd(x) + val, getl(x), getr(x));
		ll c = (lb + ub) / 2;
		auto nl = fix(getl(x), l, r, val, lb, c);
		auto nr = fix(getr(x), l, r, val, c, ub);
		return new_node(getval(x) + val * (min(r, ub) - max(l, lb)), getadd(x), nl, nr);
	}
	T sub(node<T> *x, ll l, ll r, ll lb, ll ub) {
		if (x == nullptr) return id;
		if (ub <= l || r <= lb) return id;
		if (l <= lb && ub <= r) return x->val + x->add * (ub - lb);
		ll c = (lb + ub) / 2;
		return sub(getl(x), l, r, lb, c) + sub(getr(x), l, r, c, ub) + x->add * (min(r, ub) - max(l, lb));
	}
public:
	SaikyoSegmentTree(ll n_, T id_) : n(size(n_)), id(id_), it(0) {
		root.push_back(nullptr);
	}
	void add(ll l, ll r, T val, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		root.push_back(fix(root[rt], l, r + 1, val, 0, n));
	}
	T find(ll l, ll r, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		return sub(root[rt], l, r + 1, 0, n);
	}
};

// generalized
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
	void init(T val_, T lazy_, node<T>* l_ = nullptr, node<T>* r_ = nullptr) {
		val = val_;
		lazy = lazy_;
		l = l_;
		r = r_;
	}
};

template <typename M>
class SaikyoSegmentTree {
	using T = typename M::type;
	const ll n;
	vector<node<T>*> root;
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
	node<T> *fix(node<T> *t) {
		pool[it] = *t;
		return &pool[it++];
	}
	node<T> *push(node<T> *t, ll lb, ll ub) {
		t = fix(t);
		if (lb + 1 < ub) {
			if (t->l == nullptr) t->l = new_node();
			else t->l = fix(t->l);
			if (t->r == nullptr) t->r = new_node();
			else t->r = fix(t->r);
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
	node<T> *sub(ll l, ll r, node<T> *t, ll lb, ll ub, T& res) {
		if (t == nullptr || ub <= l || r <= lb) return t;
		t = push(t, lb, ub);
		if (l <= lb && ub <= r) {
			res = M::op1(res, t->val);
			return t;
		}
		int c = (lb + ub) / 2;
		t->l = sub(l, r, t->l, lb, c, res);
		t->r = sub(l, r, t->r, c, ub, res);
		return t;
	}
public:
	SaikyoSegmentTree(ll n_, int PMAX = 2e7)
		: n(size(n_)), root(1, nullptr), pool(PMAX), it(0) {}
	void update(ll l, ll r, T val, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		root.push_back(suc(l, r + 1, root[rt], 0, n, val));
	}
	T find(ll l, ll r, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		T res = M::id1();
		root.push_back(sub(l, r + 1, root[rt], 0, n, res));
		return res;
	}
	int getversion() {
		return root.size() - 1;
	}
};
