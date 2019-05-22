
using graph = vector<vector<int>>;

graph compress_graph(const graph& g, const vector<int>& prj) {
	int n = g.size(), m = *max_element(prj.begin(), prj.end()) + 1;
	graph h(m);
	vector<unordered_set<int>> es(m);
	for (int v = 0; v < n; v++) {
		for (int w : g[v]) {
			int a = prj[v], b = prj[w];
			if (prj[v] == prj[w]) continue; // self-loop edge
			if (es[a].count(b)) continue; // multiple edge
			h[a].push_back(b); es[a].insert(b);
		}
	}
	return h;
}
