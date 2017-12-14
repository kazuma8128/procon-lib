
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

vector<int> articulation_point(const graph& g) {
	int n = g.size(), idx = 0;
	vector<int> low(n), ord(n), art;
	function<void(int)> dfs = [&](int v) {
		low[v] = ord[v] = ++idx;
		for (auto& e : g[v]) {
			int w = e.to;
			if (ord[w] == 0) {
				dfs(w);
				low[v] = min(low[v], low[w]);
				if ((ord[v] == 1 && ord[w] != 2) || (ord[v] != 1 && low[w] >= ord[v]))
					art.push_back(v);
			}
			else
				low[v] = min(low[v], ord[w]);
		}
	};
	for (int u = 0; u < n; ++u) {
		if (ord[u] == 0) {
			idx = 0;
			dfs(u);
		}
	}
	sort(art.begin(), art.end());
	art.erase(unique(art.begin(), art.end()), art.end());
	return art;
}
