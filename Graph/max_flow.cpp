
const int INF = 1e9;

class max_flow {
	struct edge {
		int to, cap, rev;
		edge(int to_, int cap_, int rev_) : to(to_), cap(cap_), rev(rev_) {};
	};

	int V;
	vector<vector<edge>> G;
	vector<int> level;
	vector<int> iter;

	void BFS(int s) {
		fill(level.begin(), level.end(), -1);
		queue<int> que;
		level[s] = 0;
		que.push(s);
		while (!que.empty()) {
			int v = que.front(); que.pop();
			for (size_t i = 0; i < G[v].size(); i++) {
				edge &e = G[v][i];
				if (e.cap > 0 && level[e.to] < 0) {
					level[e.to] = level[v] + 1;
					que.push(e.to);
				}
			}
		}
	}
	int DFS(int v, int t, int f) {
		if (v == t) return f;
		for (int &i = iter[v]; i < (int)G[v].size(); i++) {
			edge &e = G[v][i];
			if (e.cap > 0 && level[v] < level[e.to]) {
				int d = DFS(e.to, t, min(f, e.cap));
				if (d > 0) {
					e.cap -= d;
					G[e.to][e.rev].cap += d;
					return d;
				}
			}
		}
		return 0;
	}
public:
	max_flow(int _V) : V(_V), G(_V), level(_V), iter(_V) {}
	void add(int from, int to, int cap) {
		G[from].push_back(edge(to, cap, G[to].size()));
		G[to].push_back(edge(from, 0, G[from].size() - 1));
	}
	int dinic(int s, int t) {
		int flow = 0;
		while (true) {
			BFS(s);
			if (level[t] < 0) return flow;
			fill(iter.begin(), iter.end(), 0);
			int f;
			while ((f = DFS(s, t, INF)) > 0) {
				flow += f;
			}
		}
	}
};
