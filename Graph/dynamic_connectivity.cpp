
class retroactive_uf {
	vector<int> data;
	vector<pair<int, int>> hist;
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
		hist.emplace_back(b, data[b]);
		data[b] = data[a] + data[b];
		hist.emplace_back(a, data[a]);
		data[a] = b;
	}
	int size(int a) const {
		return -data[find(a)];
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
	void add_edge(int l, int r, int u, int val) {
		l += n, r += n;
		while (l < r) {
			if (l & 1) data[l++].emplace_back(u, val);
			if (r & 1) data[--r].emplace_back(u, val);
			l >>= 1, r >>= 1;
		}
	}
	void add_query(int p, int u, int val) {
		q[p].first = u;
		q[p].second = val;
	}
	vector<int> calc() {
		vector<int> res;
		retroactive_uf uuf(v);
		sub(1, res, uuf);
		return res;
	}
};
