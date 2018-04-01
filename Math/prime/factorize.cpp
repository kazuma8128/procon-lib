
vector<pair<ll, ll>> factorize(ll x) {
	vector<pair<ll, ll>> res;
	for (ll i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			res.emplace_back(i, 0);
			while (x % i == 0) {
				res.back().second++;
				x /= i;
			}
		}
	}
	if (x > 1) {
		if (!res.empty() && res.back().first == x)
			res.back().second++;
		else
			res.emplace_back(x, 1);
	}
	return res;
}
