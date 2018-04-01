
// fully persistent array
const int PMAX = 20000000;

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
int it;

template <typename T>
class persistent_array {
	const int n;
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
		if (ub - lb == 1) return new_node(val);
		int c = (lb + ub) / 2;
		if (p < c)
			return new_node(id, fix(x ? x->l : nullptr, p, val, lb, c), x ? x->r : nullptr);
		else
			return new_node(id, x ? x->l : nullptr, fix(x ? x->r : nullptr, p, val, c, ub));
	}
	T sub(node<T> *x, int p, int lb, int ub) {
		if (!x) return id;
		if (ub - lb == 1) return x->val;
		int c = (lb + ub) / 2;
		return p < c ? sub(x->l, p, lb, c) : sub(x->r, p, c, ub);
	}
public:
	persistent_array(int n_, T id_ = 0) : n(size(n_)), id(id_), root(1, nullptr) {}
	persistent_array(const vector<T>& v, T id_ = 0) : n(size(v.size())), id(id_), root(1, nullptr) {}
	T update(int p, T val, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		root.push_back(fix(root[rt], p, val, 0, n));
		return val;
	}
	T get(int p, int rt = -1) {
		if (rt == -1) rt = root.size() - 1;
		assert(0 <= rt && rt < (int)root.size());
		return sub(root[rt], p, 0, n);
	}
	int getversion() {
		return root.size() - 1;
	}
};

// partially persistent quick find
class persistent_qf {
	persistent_array<int> rt;
	vector<int> ver;
	vector<vector<int>> vs;
public:
	persistent_qf(int n) : rt(n), vs(n) {
		for (int i = 0; i < n; i++) rt.update(i, i), vs[i].push_back(i);
		ver.push_back(rt.getversion());
	}
	bool same(int a, int b, int v = -1) {
		if (v == -1) v = ver.size() - 1;
		assert(0 <= v && v < (int)ver.size());
		return rt.get(a, ver[v]) == rt.get(b, ver[v]);
	}
	void unite(int a, int b) {
		a = rt.get(a), b = rt.get(b);
		if (a == b) return;
		if (vs[a].size() < vs[b].size()) swap(a, b);
		for (auto& v : vs[b]) rt.update(v, a), vs[a].push_back(v);
		ver.push_back(rt.getversion());
	}
	int getversion() {
		return ver.size() - 1;
	}
};
