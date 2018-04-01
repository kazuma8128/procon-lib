
template<typename T>
class retroactive_array {
	vector<T> data;
	vector<pair<int, T>> hist;
public:
	retroactive_array(int N) : data(N) {}
	retroactive_array(int N, T val) : data(N, val) {}
	retroactive_array(const vector<T>& v) : data(v) {}
	retroactive_array(vector<T>&& v) : data(move(v)) {}
	void change(int k, T val) {
		hist.emplace_back(k, data[k]);
		data[k] = val;
	}
	T operator[](int k) const {
		return data[k];
	}
	int get_version() const {
		return hist.size();
	}
	void rollback(int ver) {
		assert(0 <= ver && ver <= (int)hist.size());
		int cnt = hist.size() - ver;
		while (cnt--) {
			data[hist.back().first] = hist.back().second;
			hist.pop_back();
		}
	}
};

class retroactive_uf {
	retroactive_array<int> data;
public:
	retroactive_uf(int n) : data(n, -1) {}
	int find(int a) const {
		return data[a] < 0 ? a : find(data[a]);
	}
	bool same(int a, int b) const {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (-data[a] > -data[b]) swap(a, b);
		data.change(b, data[a] + data[b]);
		data.change(a, b);
	}
	int size(int a) const {
		return -data[find(a)];
	}
	int get_version() const {
		return data.get_version();
	}
	void rollback(int ver) {
		data.rollback(ver);
	}
};

class dynamic_connectivity {
	using pii = pair<int, int>;
	const int v, n;
	vector<vector<pii>> data;
	vector<pii> q;
	int size(int x) {
		int res = 1;
		while (res < x) res <<= 1;
		return res;
	}
	void sub(int node, vector<int>& res, retroactive_uf& uuf) {
		int ver = uuf.get_version();
		for (auto& p : data[node]) uuf.unite(p.first, p.second);
		if (node >= n) {
			if (q[node - n] != pii(-1, -1)) {
				res.push_back(uuf.same(q[node - n].first, q[node - n].second));
			}
		}
		else {
			sub(node << 1, res, uuf);
			sub((node << 1) | 1, res, uuf);
		}
		uuf.rollback(ver);
	}
public:
	dynamic_connectivity(int v_, int n_) : v(v_), n(size(n_)), data(n * 2), q(n, pii(-1, -1)) {}
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
		retroactive_uf uuf(v);
		sub(1, res, uuf);
		return res;
	}
};
