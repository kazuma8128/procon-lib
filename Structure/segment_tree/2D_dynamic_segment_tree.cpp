
using ll = long long;

struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

template <typename M>
class node {
	using T = typename M::type;
public:
	T val;
	node<M> *l, *r;
	node(T val_) : val(val_), l(nullptr), r(nullptr) {}
};

template <typename M>
class dynamic_segment_tree {
	using T = typename M::type;
	const ll n;
	node<M> *root;
	T value(node<M> *t) {
		return t ? t->val : M::id();
	}
	T sub(ll l, ll r, node<M>* n, ll lb, ll ub) {
		if (!n || ub <= l || r <= lb) return M::id();
		if (l <= lb && ub <= r) return n->val;
		ll c = (lb + ub) / 2;
		return M::op(sub(l, r, n->l, lb, c), sub(l, r, n->r, c, ub));
	}
	node<M> *suc(ll p, node<M>* t, ll lb, ll ub, T val) {
		if (!t) t = new node<M>(M::id());
		if (lb + 1 == ub) {
			t->val = val;
			return t;
		}
		ll c = (lb + ub) / 2;
		if (p < c) t->l = suc(p, t->l, lb, c, val);
		else t->r = suc(p, t->r, c, ub, val);
		t->val = M::op(value(t->l), value(t->r));
		return t;
	}
public:
	dynamic_segment_tree(ll n_) :
		n(1 << (int)ceil(log2(n_))), root(nullptr) {}
	void update(ll p, T val) {
		root = suc(p, root, 0, n, val);
	}
	T find(ll l, ll r) {
		return sub(l, r + 1, root, 0, n);
	}
};

template <typename M>
class node2 {
	using T = typename M::type;
public:
	dynamic_segment_tree<M> val;
	node2<M> *l, *r;
	node2(ll size) : val(size), l(nullptr), r(nullptr) {}
};

template <typename M>
class dynamic_segment_tree2 {
	using T = typename M::type;
	const ll h, w;
	node2<M> *root;
	T value(node2<M> *t, ll p) {
		return t ? t->val.find(p, p) : M::id();
	}
	T sub(ll li, ll lj, ll ri, ll rj, node2<M>* n, ll lb, ll ub) {
		if (!n || ub <= li || ri <= lb) return M::id();
		if (li <= lb && ub <= ri) return n->val.find(lj, rj);
		ll c = (lb + ub) / 2;
		return M::op(sub(li, lj, ri, rj, n->l, lb, c), sub(li, lj, ri, rj, n->r, c, ub));
	}
	node2<M> *suc(ll pi, ll pj, node2<M>* t, ll lb, ll ub, T val) {
		if (!t) t = new node2<M>(w);
		if (lb + 1 == ub) {
			t->val.update(pj, val);
			return t;
		}
		ll c = (lb + ub) / 2;
		if (pi < c) t->l = suc(pi, pj, t->l, lb, c, val);
		else t->r = suc(pi, pj, t->r, c, ub, val);
		t->val.update(pj, M::op(value(t->l, pj), value(t->r, pj)));
		return t;
	}
public:
	dynamic_segment_tree2(ll h_, ll w_) :
		h(1ll << (ll)ceil(log2(h_))), w(w_), root(nullptr) {}
	void update(ll pi, ll pj, T val) {
		root = suc(pi, pj, root, 0, h, val);
	}
	T find(ll li, ll lj, ll ri, ll rj) {
		return sub(li, lj, ri + 1, rj, root, 0, h);
	}
};
