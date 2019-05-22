
using graph = vector<vector<int>>;

struct edge {
	int from, to;
	edge(int a, int b) : from(a), to(b) {}
};

bool operator < (const edge& l, const edge& r) {
	if (min(l.from, l.to) != min(r.from, r.to))
		return min(l.from, l.to) < min(r.from, r.to);
	return max(l.from, l.to) < max(r.from, r.to);
}

void visit(const graph &g, int v,
	vector<int>& art, vector<set<edge>>& bcomp,
	stack<edge>& S, vector<int>& num, vector<int>& low, int& time) {
	low[v] = num[v] = ++time;
	for (int w : g[v]) {
		if (num[w] < num[v]) S.emplace(v, w);
		if (num[w] == 0) {
			visit(g, w, art, bcomp, S, num, low, time);
			low[v] = min(low[v], low[w]);
			if ((num[v] == 1 && num[w] != 2) ||
				(num[v] != 1 && low[w] >= num[v])) art.push_back(v);
			if (low[w] >= num[v]) {
				bcomp.push_back(set<edge>());
				while (1) {
					edge f = S.top(); S.pop();
					bcomp.back().insert(f);
					if (f.from == v && f.to == w) break;
				}
			}
		}
		else low[v] = min(low[v], num[w]);
	}
}

pair<vector<int>, vector<set<edge>>> biconnected_component(const graph& g) {
	const int n = g.size();
	vector<int> art;
	vector<set<edge>> bcomp;
	vector<int> low(n), num(n);
	stack<edge> S;
	for (int u = 0; u < n; u++) if (num[u] == 0) {
		int time = 0;
		visit(g, u, art, bcomp, S, num, low, time);
	}
	sort(art.begin(), art.end());
	art.erase(unique(art.begin(), art.end()), art.end());
	return make_pair(art, bcomp);
}
