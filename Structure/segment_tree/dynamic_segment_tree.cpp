
using ll = long long;

struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

template <typename T>
class node {
public:
	T val;
	node<T> *l, *r;
	node(T val_) : val(val_), l(nullptr), r(nullptr) {}
	~node() {
		if (l) delete l;
		if (r) delete r;
	}
};

template <typename M>
class dynamic_segment_tree {
	using T = typename M::type;
	const ll n;
	node<T> *root;
	T value(node<T> *t) {
		return t ? t->val : M::id();
	}
	T sub(ll l, ll r, node<T>* n, ll lb, ll ub) {
		if (!n || ub <= l || r <= lb) return M::id();
		if (l <= lb && ub <= r) return n->val;
		ll c = (lb + ub) / 2;
		return M::op(sub(l, r, n->l, lb, c), sub(l, r, n->r, c, ub));
	}
	node<T> *suc(ll p, node<T>* t, ll lb, ll ub, T val) {
		if (!t) t = new node<T>(M::id());
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
	~dynamic_segment_tree() {
		if (root) delete root;
	}
	void update(ll p, T val) {
		root = suc(p, root, 0, n, val);
	}
	T find(ll l, ll r) {
		return sub(l, r + 1, root, 0, n);
	}
};
