
// non-recursive
class heavy_light_decomposition {
	const int n;
	vector<vector<int>> g;
	vector<int> par, head, in, out;
	void dfs1(int rt) {
		vector<int> size(n, 1);
		vector<size_t> iter(n);
		vector<pair<int, int>> stp;
		stp.reserve(n);
		stp.emplace_back(rt, -1);
		while (!stp.empty()) {
			int v = stp.back().first;
			if (iter[v] < g[v].size()) {
				if (g[v][iter[v]] != stp.back().second) {
					stp.emplace_back(g[v][iter[v]], v);
				}
				++iter[v];
				continue;
			}
			par[v] = stp.back().second;
			for (auto& u : g[v]) if (u == par[v]) {
				if (u != g[v].back()) swap(u, g[v].back());
				g[v].pop_back();
				break;
			}
			for (auto& u : g[v]) {
				size[v] += size[u];
				if (size[u] > size[g[v].front()]) swap(u, g[v].front());
			}
			stp.pop_back();
		}
	}
	void dfs2(int rt) {
		int it = 0;
		vector<size_t> iter(n);
		vector<int> st; st.reserve(n);
		st.push_back(rt);
		while (!st.empty()) {
			int v = st.back();
			if (!iter[v]) in[v] = it++;
			if (iter[v] < g[v].size()) {
				int u = g[v][iter[v]];
				head[u] = iter[v] ? u : head[v];
				st.push_back(u);
				++iter[v];
				continue;
			}
			out[v] = it;
			st.pop_back();
		}
	}
public:
	heavy_light_decomposition(int n_)
		: n(n_), g(n), par(n), head(n), in(n), out(n) {}
	heavy_light_decomposition(const vector<vector<int>>& G)
		: n(G.size()), g(G), par(n), head(n), in(n), out(n) {}
	void add_edge(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}
	void build(int rt = 0) {
		dfs1(rt);
		head[rt] = rt;
		dfs2(rt);
	}
	int get_id(int v) {
		return in[v];
	}
	void path_query(int u, int v, function<void(int, int)> f) {
		while (true) {
			if (in[u] > in[v]) swap(u, v);
			f(max(in[head[v]], in[u]), in[v] + 1);
			if (head[u] == head[v]) return;
			v = par[head[v]];
		}
	}
	void path_query(int u, int v, function<void(int, int, bool)> f) {
		while (true) {
			if (in[u] <= in[v]) {
				f(max(in[head[v]], in[u]), in[v] + 1, false);
				if (head[u] == head[v]) return;
				v = par[head[v]];
			}
			else {
				f(max(in[head[u]], in[v]), in[u] + 1, true);
				if (head[u] == head[v]) return;
				u = par[head[u]];
			}
		}
	}
	void subtree_query(int v, function<void(int, int)> f) {
		f(in[v], out[v]);
	}
	int get_lca(int u, int v) {
		while (true) {
			if (in[u] > in[v]) swap(u, v);
			if (head[u] == head[v]) return u;
			v = par[head[v]];
		}
	}
};

// recursive
class heavy_light_decomposition {
	const int n;
	vector<vector<int>> g;
	vector<int> in, out, size, head, par;
	int it;
	void erase_par(int v, int prev) {
		par[v] = prev;
		for (auto& u : g[v]) {
			if (u == g[v].back()) break;
			if (u == prev) swap(u, g[v].back());
			erase_par(u, v);
		}
		g[v].pop_back();
	}
	void dfs1(int v) {
		for (auto& u : g[v]) {
			dfs1(u);
			size[v] += size[u];
			if (size[u] > size[g[v][0]]) swap(u, g[v][0]);
		}
	}
	void dfs2(int v) {
		in[v] = it++;
		for (auto u : g[v]) {
			head[u] = (u == g[v][0] ? head[v] : u);
			dfs2(u);
		}
		out[v] = it;
	}
public:
	heavy_light_decomposition(int n_)
		: n(n_), g(n), in(n, -1), out(n, -1), size(n, 1), head(n), par(n, -1), it(0) {}
	heavy_light_decomposition(const vector<vector<int>>& G)
		: n(G.size()), g(G), in(n, -1), out(n, -1), size(n, 1), head(n), par(n, -1), it(0) {}
	void add_edge(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}
	void build(int rt = 0) {
		for (auto v : g[rt]) erase_par(v, rt);
		dfs1(rt);
		head[rt] = rt;
		dfs2(rt);
	}
	int get_id(int v) {
		return in[v];
	}
	void path_query(int u, int v, function<void(int, int)> f) {
		while (true) {
			if (in[u] > in[v]) swap(u, v);
			f(max(in[head[v]], in[u]), in[v] + 1);
			if (head[u] == head[v]) return;
			v = par[head[v]];
		}
	}
	void path_query(int u, int v, function<void(int, int, bool)> f) {
		while (true) {
			if (in[u] <= in[v]) {
				f(max(in[head[v]], in[u]), in[v] + 1, false);
				if (head[u] == head[v]) return;
				v = par[head[v]];
			}
			else {
				f(max(in[head[u]], in[v]), in[u] + 1, true);
				if (head[u] == head[v]) return;
				u = par[head[u]];
			}
		}
	}
	void edge_path_query(int u, int v, function<void(int, int)> f) {
		while (true) {
			if (in[u] > in[v]) swap(u, v);
			if (head[u] != head[v]) {
				f(max(in[head[v]], in[u]), in[v] + 1);
				v = par[head[v]];
			}
			else {
				if (u != v) f(in[u] + 1, in[v] + 1);
				break;
			}
		}
	}
	void edge_path_query(int u, int v, function<void(int, int, bool)> f) {
		while (true) {
			if (in[u] > in[v]) {
				if (head[u] != head[v]) {
					f(max(in[head[u]], in[v]), in[u] + 1, true);
					u = par[head[u]];
				}
				else {
					if (u != v) f(in[v] + 1, in[u] + 1, false);
					break;
				}
			}
			else {
				if (head[u] != head[v]) {
					f(max(in[head[v]], in[u]), in[v] + 1, false);
					v = par[head[v]];
				}
				else {
					if (u != v) f(in[u] + 1, in[v] + 1, false);
					break;
				}
			}
		}
	}
	void subtree_query(int v, function<void(int, int)> f) {
		f(in[v], out[v]);
	}
	int get_lca(int u, int v) {
		while (true) {
			if (in[u] > in[v]) swap(u, v);
			if (head[u] == head[v]) return u;
			v = par[head[v]];
		}
	}
};
