
struct edge {
	int from, to;
	edge(int f, int t) : from(f), to(t) {}
};

using edges = vector<edge>;
using graph = vector<edges>;

void add_edge(graph& G, int f, int t) {
	G[f].emplace_back(f, t);
	G[t].emplace_back(t, f);
}

pair<vector<int>, edges> bridge(const graph& G) {
	const int n = G.size();
	int idx = 0, t = 0, k = 0;
	vector<int> ord(n, -1), stk, roots(n), cmp(n);
	vector<bool> onS(n);
	edges brdg;
	function<void(int, int)> dfs = [&](int v, int u) {
		ord[v] = idx++;
		stk.push_back(v);
		onS[v] = true;
		roots[t++] = v;
		for (auto& e : G[v]) {
			int w = e.to;
			if (ord[w] == -1) dfs(w, v);
			else if (u != w && onS[w])
				while (ord[roots[t - 1]] > ord[w]) --t;
		}
		if (v == roots[t - 1]) {
			brdg.emplace_back(u, v);
			while (true) {
				int w = stk.back(); stk.pop_back();
				onS[w] = false;
				cmp[w] = k;
				if (v == w) break;
			}
			--t;
			++k;
		}
	};
	for (int u = 0; u < n; ++u) {
		if (ord[u] == -1) {
			dfs(u, -1);
			brdg.pop_back();
		}
	}
	return make_pair(cmp, brdg);
}
