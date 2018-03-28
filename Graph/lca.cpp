
// Euler Tour + Sparse Table ver
using graph = vector<vector<int>>;

template <typename T>
class SparseTable {
	const int n, h;
	vector<vector<T>> t;
public:
	SparseTable(const vector<T>& b)
		: n(b.size()), h(31 - __builtin_clz(n)), t(h + 1, vector<T>(b.size())) {
		t[0] = b;
		for (int i = 0; i < h; i++)
			for (int k = 0; k + (1 << i) < n; k++)
				t[i + 1][k] = min(t[i][k], t[i][k + (1 << i)]);
	}
	T find(int l, int r) {
		int b = 31 - __builtin_clz(r - l);
		return min(t[b][l], t[b][r - (1 << b)]);
	}
};

class LCA {
	using pii = pair<int, int>;
	vector<int> depth, id;
	SparseTable<pii> st;
	void dfs(int v, int prev, const graph& G, vector<pii>& res) {
		depth[v] = prev != -1 ? depth[prev] + 1 : 0;
		id[v] = res.size();
		res.emplace_back(depth[v], v);
		for (auto to : G[v]) if (to != prev) {
			dfs(to, v, G, res);
			res.emplace_back(depth[v], v);
		}
	}
	vector<pii> init(const graph& G, int rt) {
		vector<pii> res;
		res.reserve(G.size() * 2 - 1);
		dfs(rt, -1, G, res);
		return res;
	}
public:
	LCA(const graph& G, int rt = 0) : depth(G.size()), id(G.size()), st(init(G, rt)) {}
	int calc(int a, int b) {
		if (id[a] > id[b]) swap(a, b);
		return st.find(id[a], id[b] + 1).second;
	}
	int dist(int a, int b) {
		return depth[a] + depth[b] - depth[calc(a, b)] * 2;
	}
};

// HL-Decomposition ver
class LCA {
	using pii = pair<int, int>;
	vector<int> depth, par, heavy, head;
	int dfs(int v, int prev, const graph& G) {
		par[v] = prev;
		depth[v] = prev != -1 ? depth[prev] + 1 : 0;
		int ma = 0, cnt = 1;
		for (auto to : G[v]) if (to != prev) {
			int c = dfs(to, v, G);
			if (c > ma) {
				ma = c;
				heavy[v] = to;
			}
			cnt += c;
		}
		return cnt;
	}
public:
	LCA(const graph& G, int rt = 0)
		: depth(G.size()), par(G.size()), heavy(G.size(), -1), head(G.size()) {
		dfs(rt, -1, G);
		queue<int> q;
		q.push(rt);
		while (!q.empty()) {
			int h = q.front(); q.pop();
			for (int v = h; v != -1; v = heavy[v]) {
				head[v] = h;
				for (auto to : G[v]) if (to != par[v] && to != heavy[v]) {
					q.push(to);
				}
			}
		}
	}
	int calc(int u, int v) {
		while (head[u] != head[v]) {
			if (depth[head[u]] < depth[head[v]]) v = par[head[v]];
			else u = par[head[u]];
		}
		return depth[u] < depth[v] ? u : v;
	}
	int dist(int a, int b) {
		return depth[a] + depth[b] - depth[calc(a, b)] * 2;
	}
};
