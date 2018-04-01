
// fully persistent array (random access : O(logN))
const int PMAX = 10000000;

template <typename T>
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

node<int> pool[PMAX];

template <typename T>
class persistent_array {
	const int n;
	int it;
	const T id;
	vector<node<T>*> root;
	int size(int n) {
		int res = 1;
		while (res < n) res <<= 1;
		return res;
	}
	node<T> *new_node(int val = 0, node<T> *l = nullptr, node<T> *r = nullptr) {
		pool[it].init(val, l, r);
		return &pool[it++];
	}
	node<T> *fix(node<T> *x, int p, T val, int lb, int ub) {
		if (p < lb || ub <= p) return x;
		if (lb + 1 == ub) return new_node(val);
		int c = (lb + ub) / 2;
		auto nl = fix(x->l, p, val, lb, c);
		auto nr = fix(x->r, p, val, c, ub);
		return new_node(id, nl, nr);
	}
	T sub(node<T> *x, int p, int lb, int ub) {
		if (lb + 1 == ub) return x->val;
		int c = (lb + ub) / 2;
		if (p < c) return sub(x->l, p, lb, c);
		return sub(x->r, p, c, ub);
	}
public:
	persistent_array(int n_, T id_ = 0) : n(size(n_)), id(id_) {
		for (int i = 1; i < n * 2; i++) {
			pool[i - 1].init(id, i < n ? &(pool[i * 2 - 1]) : nullptr, i < n ? &(pool[i * 2]) : nullptr);
		}
		it = n * 2 - 1;
		root.push_back(&pool[0]);
	}
	persistent_array(const vector<T>& v, T id_ = 0) : n(size(v.size())), id(id_) {
		it = 0;
		for (int i = n * 2 - 1; i > 0; i--) {
			if (i >= n && i - n < (int)v.size()) {
				pool[it++].init(v[i - n], nullptr, nullptr);
			}
			else if (i < n) {
				pool[it++].init(id, &(pool[i * 2 - 1]), &(pool[i * 2]));
			}
			else {
				pool[it++].init(id, nullptr, nullptr);
			}
		}
		root.push_back(&pool[0]);
	}
	void update(int p, T val, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		root.push_back(fix(root[rt], p, val, 0, n));
	}
	T find(int p, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		return sub(root[rt], p, 0, n);
	}
	int getversion() {
		return root.size() - 1;
	}
};
