
const int MAX = 1e7;

vector<pair<int, int>> factorize(int x) {
	bool ok = false;
	static int ps[MAX + 1];
	assert(x <= MAX);
	if (!ok) {
		for (int i = 2; i <= MAX; i++) if (!ps[i]) {
			for (int j = i; j <= MAX; j += i) if (!ps[j]) {
				ps[j] = i;
			}
		}
		ok = true;
	}
	vector<pair<int, int>> res;
	while (x > 1) {
		int p = ps[x];
		res.emplace_back(p, 0);
		while (x % p == 0) x /= p, ++res.back().second;
	}
	return res;
}
