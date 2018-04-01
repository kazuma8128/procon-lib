
class heavy_light_decomposition {
	vector<vector<int>> G;
	vector<int> vid, head, heavy, par, dep;
	int dfs(int v, int prev) {
		par[v] = prev;
		int cnt = 1, ma = 0;
		for (int to : G[v]) if (to != prev) {
			dep[to] = dep[v] + 1;
			int c = dfs(to, v);
			cnt += c;
			if (ma < c) {
				ma = c;
				heavy[v] = to;
			}
		}
		return cnt;
	}
public:
	heavy_light_decomposition(int n) : G(n), vid(n, -1), head(n), heavy(n, -1), par(n), dep(n) {}
	void add(int u, int v) {
		G[u].push_back(v);
		G[v].push_back(u);
	}
	void build() {
		dfs(0, -1);
		int k = 0;
		queue<int> q;
		q.push(0);
		while (!q.empty()) {
			int h = q.front(); q.pop();
			for (int i = h; i != -1; i = heavy[i]) {
				vid[i] = k++;
				head[i] = h;
				for (int j : G[i]) if (j != par[i] && j != heavy[i]) q.push(j);
			}
		}
	}
	int operator[](int v) {
		return vid[v];
	}
	void for_each(int u, int v, function<void(int, int)> f) {
		while (true) {
			if (vid[u] > vid[v]) swap(u, v);
			f(max(vid[head[v]], vid[u]), vid[v]);
			if (head[u] == head[v]) break;
			v = par[head[v]];
		}
	}
	void for_each_directed(int u, int v, function<void(int, int, int)> f) {
		while (true) {
			if (vid[u] > vid[v]) {
				f(max(vid[head[u]], vid[v]), vid[u], 1);
				if (head[u] == head[v]) break;
				u = par[head[u]];
			}
			else {
				f(max(vid[head[v]], vid[u]), vid[v], 0);
				if (head[u] == head[v]) break;
				v = par[head[v]];
			}
		}
	}
	void for_each_edge(int u, int v, function<void(int, int)> f) {
		while (true) {
			if (vid[u] > vid[v]) swap(u, v);
			if (head[u] != head[v]) {
				f(vid[head[v]], vid[v]);
				v = par[head[v]];
			}
			else {
				if (u != v) f(vid[u] + 1, vid[v]);
				break;
			}
		}
	}
	void for_each_edge_directed(int u, int v, function<void(int, int, int)> f) {
		while (true) {
			if (vid[u] > vid[v]) {
				if (head[v] != head[u]) {
					f(vid[head[u]], vid[u], 1);
					u = par[head[u]];
				}
				else {
					if (v != u) f(vid[v] + 1, vid[u], 1);
					break;
				}
			}
			else {
				if (head[u] != head[v]) {
					f(vid[head[v]], vid[v], 0);
					v = par[head[v]];
				}
				else {
					if (u != v) f(vid[u] + 1, vid[v], 0);
					break;
				}
			}
		}
	}
	int ancestor(int u, int d) {
		while (true) {
			if (dep[head[u]] > dep[u] - d) {
				d -= dep[u] - dep[head[u]] + 1;
				if (head[u] == 0) return 0;
				u = par[head[u]];
			}
			else {
				return inv[vid[u] - d];
			}
		}
	}
	int lca(int u, int v) {
		while (true) {
			if (vid[u] > vid[v]) swap(u, v);
			if (head[u] == head[v]) return u;
			v = par[head[v]];
		}
	}
	int distance(int u, int v) {
		return dep[u] + dep[v] - 2 * dep[lca(u, v)];
	}
};
