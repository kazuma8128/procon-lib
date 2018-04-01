
// function ver
struct edge {
	int from, to;
	int cost;
	edge(int f, int t, int c) : from(f), to(t), cost(c) {}
};

using edges = vector<edge>;
using graph = vector<edges>;

void add_edge(graph& G, int s, int t, int c) {
	G[s].emplace_back(s, t, c);
}

template <typename C, const C inf>
vector<C> bellman_ford(const graph& G, int s) {
	int n = G.size();
	vector<C> d(n, inf);
	d[s] = C();
	bool update;
	for (int i = 0; i < n; i++) {
		update = false;
		for (auto& es : G) {
			for (auto e : es) {
				if (d[e.from] == inf || d[e.to] <= d[e.from] + e.cost) continue;
				d[e.to] = d[e.from] + e.cost;
				update = true;
			}
		}
	}
	return update ? vector<C>() : d;
}

// class ver
template <typename C, const C inf>
class bellman_ford {
	struct edge {
		int from, to;
		C cost;
		edge(int _from, int _to, int _cost) : from(_from), to(_to), cost(_cost) {}
	};
	vector<edge> es;
	bool isdir;
	vector<C> d;
	int V;
public:
	bellman_ford(int _V, bool _isdirected = true) : isdir(_isdirected), V(_V), d(_V) { }
	void add(int a, int b, C c) {
		es.emplace_back(a, b, c);
		if (!isdir) es.emplace_back(b, a, c);
	}
	bool calc(int s) {
		fill(d.begin(), d.end(), inf);
		d[s] = C();
		bool update;
		for (int i = 0; i < V; i++) {
			update = false;
			for (auto &e : es) {
				if (d[e.from] == inf || d[e.to] <= d[e.from] + e.cost) continue;
				d[e.to] = d[e.from] + e.cost;
				update = true;
			}
		}
		return update;
	}
	vector<C> dist() const {
		return d;
	}
};
