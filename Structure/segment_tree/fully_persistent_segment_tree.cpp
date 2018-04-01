
struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

const int PMAX = 1e7;

template <typename M>
class persistent_segment_tree {
	using T = typename M::type;
	struct node {
		T val;
		node *l, *r;
		node() {}
		node(T val) : val(val), l(nullptr), r(nullptr) {}
		void init(T v, node *left, node *right) {
			val = v;
			l = left;
			r = right;
		}
	};
	const int n;
	vector<node> pool;
	int it;
	vector<node*> root;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	T get(node *p) {
		return p == nullptr ? M::id() : p->val;
	}
	node *new_node(int val = 0, node *l = nullptr, node *r = nullptr) {
		pool[it].init(val, l, r);
		return &pool[it++];
	}
	node *fix(node *x, int p, T val, int lb, int ub) {
		if (p < lb || ub <= p) return x;
		if (lb + 1 == ub) return new_node(val);
		if (x == nullptr) x = new_node(M::id());
		else x = new_node(x->val, x->l, x->r);
		int c = (lb + ub) / 2;
		x->l = fix(x->l, p, val, lb, c);
		x->r = fix(x->r, p, val, c, ub);
		x->val = M::op(get(x->l), get(x->r));
		return x;
	}
	T sub(node *x, int l, int r, int lb, int ub) {
		if (x == nullptr || ub <= l || r <= lb) return M::id();
		if (l <= lb && ub <= r) return x->val;
		int c = (lb + ub) / 2;
		return M::op(sub(x->l, l, r, lb, c), sub(x->r, l, r, c, ub));
	}
public:
	persistent_segment_tree(int n_) : n(size(n_)), pool(PMAX), it(0) {
		root.push_back(nullptr);
	}
	void update(int p, T val, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		root.push_back(fix(root[rt], p, val, 0, n));
	}
	T find(int l, int r, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		return sub(root[rt], l, r + 1, 0, n);
	}
	int getversion() {
		return root.size() - 1;
	}
};
