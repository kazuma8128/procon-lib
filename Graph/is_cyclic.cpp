
using graph = vector<vector<int>>;

bool is_cyclic(const graph& G) {
	int n = G.size();
	vector<bool> used(n), on(n);
	function<bool(int)> dfs = [&](int v) {
		used[v] = on[v] = true;
		for (int w : G[v]) {
			if (on[w]) return true;
			if (!used[w] && dfs(w)) return true;
		}
		on[v] = false;
		return false;
	};
	for (int i = 0; i < n; i++) {
		if (!used[i] && dfs(i)) return true;
	}
	return false;
}
