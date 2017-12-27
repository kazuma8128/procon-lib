
using pii = pair<int, int>;

template<typename T>
class UndoableArray {
	int N;
	vector<int> pos, pre, data;
public:
	UndoableArray(int N) : N(N), data(N) {}
	UndoableArray(int N, T val) : N(N), data(N, val) {}
	UndoableArray(const vector<T>& v) : N(v.size()), data(v) {}
	UndoableArray(vector<T>&& v) : N(v.size()), data(move(v)) {}
	T change(int k, T val) {
		pos.push_back(k);
		pre.push_back(data[k]);
		return data[k] = val;
	}
	void undo() {
		data[pos.back()] = pre.back();
		pos.pop_back(), pre.pop_back();
	}
	T operator[](int k) const {
		return data[k];
	}
	int getversion() const {
		return pos.size();
	}
};

class UndoableUnionFind {
	UndoableArray<int> data;
	vector<int> pre;
	int find(int a) {
		return data[a] < 0 ? a : data.change(a, find(data[a]));
	}
public:
	UndoableUnionFind(int n) : data(n, -1) {}
	bool same(int a, int b) {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		pre.push_back(data.getversion());
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (data[a] < data[b]) swap(a, b);
		data.change(b, data[a] + data[b]);
		data.change(a, b);
	}
	void undo() {
		int cnt = data.getversion() - pre.back();
		while (cnt--) data.undo();
		pre.pop_back();
	}
	int size(int a) {
		return -data[find(a)];
	}
	int getversion() {
		return pre.size();
	}
};

class DynamicConnectivity {
	const int v, n;
	vector<vector<pii>> data;
	vector<pii> q;
	int size(int x) {
		int res = 1;
		while (res < x) res <<= 1;
		return res;
	}
	void sub(int node, vector<int>& res, UndoableUnionFind& uuf) {
		for (auto p : data[node]) uuf.unite(p.first, p.second);
		if (node < n) {
			sub(node * 2, res, uuf);
			sub(node * 2 + 1, res, uuf);
		}
		else if (q[node - n] != pii(-1, -1)) {
			res.push_back(uuf.same(q[node - n].first, q[node - n].second));
		}
		for (int i = 0; i < (int)data[node].size(); i++) uuf.undo();
	}
public:
	DynamicConnectivity(int v_, int n_) : v(v_), n(size(n_)), data(n * 2), q(n, pii(-1, -1)) {}
	void add_edge(int l, int r, int u, int v) {
		l += n, r += n;
		while (l < r) {
			if (l & 1) data[l++].emplace_back(u, v);
			if (r & 1) data[--r].emplace_back(u, v);
			l >>= 1, r >>= 1;
		}
	}
	void add_query(int p, int u, int v) {
		q[p].first = u;
		q[p].second = v;
	}
	vector<int> calc() {
		vector<int> res;
		UndoableUnionFind uuf(v);
		sub(1, res, uuf);
		return res;
	}
};
