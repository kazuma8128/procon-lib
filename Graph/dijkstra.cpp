
// function ver
using Cost = int;
const Cost INF = 2e9;

struct Edge {
	int from, to;
	Cost cost;
	Edge() {}
	Edge(int f, int t, Cost c) : from(f), to(t), cost(c) {}
};

using Edges = vector<Edge>;
using Graph = vector<Edges>;

vector<Cost> Dijkstra(const Graph& G, int s) {
	int n = G.size();
	vector<Cost> d(n, INF);
	using pci = pair<Cost, int>;
	priority_queue<pci, vector<pci>, greater<pci>> pq;
	pq.emplace(0, s);
	d[s] = 0;
	while (!pq.empty()) {
		auto p = pq.top(); pq.pop();
		if (p.first > d[p.second]) continue;
		for (const auto& e : G[p.second]) if (d[e.to] > p.first + e.cost) {
			pq.emplace(p.first + e.cost, e.to);
			d[e.to] = p.first + e.cost;
		}
	}
	return d;
}

// class ver
using Cost = int;
const Cost inf = 1e9;

class Dijkstra {
	struct edge {
		int to;
		Cost cost;
		edge(int to_, Cost cost_) : to(to_), cost(cost_) {}
	};
	bool isdir;
	vector<vector<edge>> G;
	int V;
public:
	Dijkstra(int V_, bool isdir_ = true) : isdir(isdir_), V(V_), G(V_) {}
	void add(int a, int b, Cost c) {
		G[a].emplace_back(b, c);
		if (!isdir) G[b].emplace_back(a, c);
	}
	vector<Cost> calc(int s) {
		vector<Cost> d(V, inf);
		using pci = pair<Cost, int>;
		priority_queue<pci, vector<pci>, greater<pci>> q;
		q.emplace(0, s);
		while (!q.empty()) {
			auto p = q.top(); q.pop();
			int v = p.second;
			if (d[v] < p.first) continue;
			d[v] = p.first;
			for (auto& e : G[v]) {
				if (d[e.to] > d[v] + e.cost) {
					d[e.to] = d[v] + e.cost;
					q.emplace(d[e.to], e.to);
				}
			}
		}
		return d;
	}
};
