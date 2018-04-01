
struct edge {
	int from, to;
	edge(int f, int t) : from(f), to(t) {}
};

using edges = vector<edge>;
using graph = vector<edges>;

vector<int> scc(const graph& g) {
	int n = g.size(), idx = 0, k = 0, s = 0;
	vector<int> ord(n, -1), low(n), onS(n), cmp(n), stk(n);
	function<void(int)> dfs;
	dfs = [&](int v) {
		ord[v] = low[v] = idx++;
		stk[s++] = v;
		onS[v] = true;
		for (auto &e : g[v]) {
			int w = e.to;
			if (ord[w] == -1) {
				dfs(w);
				low[v] = min(low[v], low[w]);
			}
			else if (onS[w]) {
				low[v] = min(low[v], ord[w]);
			}
		}
		if (low[v] == ord[v]) {
			while (true) {
				int w = stk[--s];
				onS[w] = false;
				cmp[w] = k;
				if (w == v)break;
			}
			++k;
		}
	};
	for (int v = 0; v < n; ++v)
		if (ord[v] == -1) dfs(v);
	return cmp;
}
