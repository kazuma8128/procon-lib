
template <typename C>
bool warshall_floyd(vector<vector<C>>& G) {
	int n = G.size();
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (G[i][k] == INF || G[k][j] == INF || G[i][j] <= G[i][k] + G[k][j]) continue;
				G[i][j] = G[i][k] + G[k][j];
			}
		}
	}
	for (int i = 0; i < n; i++) if (G[i][i] < C()) return true;
	return false;
}

template <typename C>
void add_edge(vector<vector<C>>& G, int s, int t, C c) {
	G[s][t] = min(G[s][t], c);
}
