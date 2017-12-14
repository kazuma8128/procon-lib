
using graph = vector<vector<int>>;

int max_clique(const graph& g) {
	int n = g.size();
	int n1 = n / 2, n2 = n - n1;
	vector<int> es1(n, 1 << n1), es2(n, 1 << n2);
	for (int i = 0; i < n; i++) {
		for (auto to : g[i]) {
			if (to < n1) {
				es1[i] ^= 1 << to;
			}
			else {
				es2[i] ^= 1 << (to - n1);
			}
		}
	}
	vector<int> dp1(1 << n1), dp2(1 << n2);
	for (int i = 0; i < (1 << n1); i++) {
		for (int j = 0; j < n1; j++) {
			if (i & (1 << j)) {
				bool ok = (i & es1[j]) == 0;
				dp1[i] = max(dp1[i], dp1[i ^ (1 << j)] + ok);
			}
		}
	}
	for (int i = 0; i < (1 << n2); i++) {
		for (int j = 0; j < n2; j++) {
			if (i & (1 << j)) {
				bool ok = (i & es2[n1 + j]) == 0;
				dp2[i] = max(dp2[i], dp2[i ^ (1 << j)] + ok);
			}
		}
	}
	int res = 0;
	for (int i = 0; i < (1 << n2); i++) {
		int now = (1 << n1) - 1;
		for (int j = 0; j < n2; j++) {
			if (i & (1 << j)) {
				now &= ~es1[n1 + j];
			}
		}
		res = max(res, dp2[i] + dp1[now]);
	}
	return res;
}
