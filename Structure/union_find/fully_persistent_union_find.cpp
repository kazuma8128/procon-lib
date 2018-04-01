
// fully persistent array
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

// fully persistent union find O(ƒ¿(N)*logN)
class persistent_uf {
	const int n;
	persistent_array<int> data;
	vector<int> ver;
	int find(int a, int v) {
		int rt = data.get(a, ver[v]);
		return rt < 0 ? a : find(rt, v);
	}
public:
	persistent_uf(int n_) : n(n_), data(n_, -1) {
		ver.push_back(data.getversion());
	}
	bool same(int a, int b, int v = -1) {
		if (v == -1) v = ver.size() - 1;
		assert(0 <= v && v < (int)ver.size());
		return find(a, v) == find(b, v);
	}
	void unite(int a, int b, int v = -1) {
		if (v == -1) v = ver.size() - 1;
		assert(0 <= v && v < (int)ver.size());
		a = find(a, v);
		b = find(b, v);
		if (a == b) return;
		int ta = data.get(a, ver[v]);
		int tb = data.get(b, ver[v]);
		if (ta < tb) {
			data.update(a, ta + tb, ver[v]);
			data.update(b, a);
		}
		else {
			data.update(b, ta + tb, ver[v]);
			data.update(a, b);
		}
		ver.push_back(data.getversion());
	}
	int size(int a, int v = -1) {
		if (v == -1) v = ver.size() - 1;
		assert(0 <= v && v < (int)ver.size());
		return -data.get(find(a, v), ver[v]);
	}
	int getversion() {
		return ver.size() - 1;
	}
};
