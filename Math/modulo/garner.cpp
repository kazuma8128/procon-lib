
using ll = long long;

ll power(ll x, int n) {
	ll res = 1;
	while (n) {
		if (n & 1) res *= x;
		x *= x;
		n >>= 1;
	}
	return res;
}

ll mod_inv(ll a, ll b) {
	ll u = 1, v = 0;
	while (b) {
		long long t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	return u;
}

vector<pair<ll, int>> factorize(ll n) {
	vector<pair<ll, int>> res;
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			res.emplace_back(i, 0);
			while (n % i == 0) {
				res.back().second++;
				n /= i;
			}
		}
	}
	if (n != 1) {
		if (!res.empty() && res.back().first == n) res.back().second++;
		else res.emplace_back(n, 1);
	}
	return res;
}

class garner {
	bool ng;
	map<ll, pair<int, ll>> facts;
public:
	garner() : ng(false) {}
	void add(ll x, ll m) {
		if (ng) return;
		for (auto& p : factorize(m)) {
			if (!facts.count(p.first)) {
				facts[p.first] = make_pair(p.second, x % power(p.first, p.second));
				continue;
			}
			auto q = facts[p.first];
			if ((x - q.second) % power(p.first, min(p.second, q.first)) != 0) {
				ng = true;
				return;
			}
			if (p.second > q.first) {
				facts[p.first] = make_pair(p.second, x % power(p.first, p.second));
			}
		}
	}
	int solve(int md) const {
		if (ng) return -1;
		bool zero = true;
		vector<pair<ll, ll>> mr;
		for (auto p : facts) {
			mr.emplace_back(power(p.first, p.second.first), p.second.second);
			if (p.second.second != 0) zero = false;
		}
		if (zero) {
			ll res = 1;
			for (auto p : facts) {
				(res *= power(p.first, p.second.first)) %= md;
			}
			return res;
		}
		mr.emplace_back(md, 0);
		int n = mr.size();
		vector<ll> coffs(n, 1);
		vector<ll> constants(n, 0);
		for (int i = 0; i < n - 1; i++) {
			ll v = (mr[i].second - constants[i]) * mod_inv(coffs[i], mr[i].first) % mr[i].first;
			if (v < 0) v += mr[i].first;
			for (int j = i + 1; j < n; j++) {
				(constants[j] += coffs[j] * v) %= mr[j].first;
				(coffs[j] *= mr[i].first) %= mr[j].first;
			}
		}
		return constants.back();
	}
};
