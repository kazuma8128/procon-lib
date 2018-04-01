
// normal
struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

template <typename M>
class starry_sky_tree {
	using T = typename M::type;
	const int n;
	vector<T> data, lazy;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	T sub(int l, int r, int node, int lb, int ub) {
		if (r <= lb || ub <= l) return M::id();
		if (l <= lb && ub <= r) return data[node];
		return M::op(sub(l, r, node << 1, lb, (lb + ub) >> 1)
			, sub(l, r, (node << 1) | 1, (lb + ub) >> 1, ub)) + lazy[node];
	}
	void suc(int l, int r, int node, int lb, int ub, T val) {
		if (ub <= l || r <= lb) return;
		if (l <= lb && ub <= r) {
			data[node] += val;
			lazy[node] += val;
			return;
		}
		suc(l, r, node << 1, lb, (lb + ub) >> 1, val);
		suc(l, r, (node << 1) | 1, (lb + ub) >> 1, ub, val);
		data[node] = M::op(data[node << 1], data[(node << 1) | 1]) + lazy[node];
	}
public:
	starry_sky_tree(int n_) : n(size(n_)), data(n << 1), lazy(n << 1) {}
	void add(int l, int r, T val) {
		suc(l, r + 1, 1, 0, n, val);
	}
	T find(int l, int r) {
		return sub(l, r + 1, 1, 0, n);
	}
};

// dynamic
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
	T val, add;
	node<M> *l, *r;
	node() : val(), add(), l(nullptr), r(nullptr) {}
};

template <typename M>
class starry_sky_tree {
	using T = typename M::type;
	const ll n;
	node<M> *root;
	ll size(ll n) {
		ll res = 1;
		while (res < n) res <<= 1ll;
		return res;
	}
	T sub(ll l, ll r, node<M> *n, ll lb, ll ub) {
		if (ub <= l || r <= lb) return M::id();
		if (!n) return T();
		if (l <= lb && ub <= r) return n->val + n->add;
		return M::op(sub(l, r, n->l, lb, (lb + ub) / 2), sub(l, r, n->r, (lb + ub) / 2, ub)) + n->add;
	}
	node<M>* suc(ll l, ll r, node<M> *n, ll lb, ll ub, T val) {
		if (ub <= l || r <= lb) return n;
		if (!n) n = new node<M>;
		if (l <= lb && ub <= r) {
			n->add += val;
			return n;
		}
		n->l = suc(l, r, n->l, lb, (lb + ub) / 2, val);
		n->r = suc(l, r, n->r, (lb + ub) / 2, ub, val);
		n->val = M::op(n->l ? n->l->val + n->l->add : T(), n->r ? n->r->val + n->r->add : T());
		return n;
	}
public:
	starry_sky_tree(ll n_) : n(size(n_)), root(nullptr) {}
	void add(ll l, ll r, T val) {
		root = suc(l, r + 1, root, 0, n, val);
	}
	T find(ll l, ll r) {
		return sub(l, r + 1, root, 0, n);
	}
};
