
const int INF = 1e9;

class min_cost_flow {
	using pii = pair<int, int>;
	struct edge {
		int to, cap, cost, rev;
		edge(int to_, int cap_, int cost_, int rev_)
			: to(to_), cap(cap_), cost(cost_), rev(rev_) {}
	};
	int V;
	vector<vector<edge>> G;
	vector<int> h, dist, prevv, preve;
public:
	min_cost_flow(int _V) : V(_V), G(_V), h(_V), dist(_V), prevv(_V), preve(_V) {}
	void add(int from, int to, int cap, int cost) {
		G[from].push_back(edge(to, cap, cost, G[to].size()));
		G[to].push_back(edge(from, 0, -cost, G[from].size() - 1));
	}
	int calc(int s, int t, int f) {
		int res = 0;
		fill(h.begin(), h.end(), 0);
		while (f > 0) {
			priority_queue<pii, vector<pii>, greater<pii>> que;
			fill(dist.begin(), dist.end(), INF);
			dist[s] = 0;
			que.push(pii(0, s));
			while (!que.empty()) {
				pii p = que.top(); que.pop();
				int v = p.second;
				if (dist[v] < p.first) continue;
				for (size_t i = 0; i < G[v].size(); i++) {
					edge &e = G[v][i];
					if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
						dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
						prevv[e.to] = v;
						preve[e.to] = i;
						que.push(pii(dist[e.to], e.to));
					}
				}
			}
			if (dist[t] == INF) return -1;
			for (int v = 0; v < V; v++)
				h[v] += dist[v];
			int d = f;
			for (int v = t; v != s; v = prevv[v]) {
				d = min(d, G[prevv[v]][preve[v]].cap);
			}
			f -= d;
			res += d * h[t];
			for (int v = t; v != s; v = prevv[v]) {
				edge &e = G[prevv[v]][preve[v]];
				e.cap -= d;
				G[v][e.rev].cap += d;
			}
		}
		return res;
	}
};
