
using graph = vector<vector<int>>;

// (0, 1, 2) = (lose, win, draw)
vector<int> retrograde(const graph& g) {
	int n = g.size();
	graph rg(n);
	vector<int> cnt(n), res(n, 2);
	queue<int> q;
	for (int i = 0; i < n; i++) {
		for (int to : g[i]) rg[to].push_back(i);
		cnt[i] = g[i].size();
		if (!cnt[i]) res[i] = 0, q.push(i);
	}
	while (!q.empty()) {
		int v = q.front(); q.pop();
		if (res[v]) {
			for (int to : rg[v]) if (res[to] == 2) {
				--cnt[to];
				if (cnt[to]) continue;
				res[to] = 0;
				q.push(to);
			}
			continue;
		}
		for (int to : rg[v]) if (res[to] == 2) res[to] = 1, q.push(to);
	}
	return res;
}
