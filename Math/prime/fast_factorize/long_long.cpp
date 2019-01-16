
using ll = long long;

ll mod_prod(ll a, ll b, ll md) {
	ll res = (a * b - (ll)((long double)a / md * b) * md) % md;
	return res < 0 ? res + md : res;
}

ll mod_pow(ll x, ll k, ll m) {
	ll res = 1;
	while (k) {
		if (k & 1) res = mod_prod(res, x, m);
		k /= 2;
		x = mod_prod(x, x, m);
	}
	return res;
}

bool miller_rabin_test(ll n) {
	const static vector<ll> sprp = {
		2, 7, 61               //       4,759,123,141
	//	2, 3, 5, 7, 11         //   2,152,302,898,747
	//	2, 3, 5, 7, 11, 13, 17 // 341,550,071,728,321
	//	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 // very large
	};
	if (n < 2) return false;
	ll d = n - 1, s = 0;
	while ((d & 1) == 0) d >>= 1, ++s;
	for (ll a : sprp) if (a <= n) {
		if (a == n) return true;
		if (mod_pow(a, d, n) != 1) {
			bool ok = true;
			for (ll r = 0; r < s; r++) {
				if (mod_pow(a, d << r, n) == n - 1) {
					ok = false;
					break;
				}
			}
			if (ok) return false;
		}
	}
	return true;
}

ll pollard_rho(ll n) {
	static mt19937 mt(random_device{}());
	if (n == 4) return 2;
	ll c = mt() % n;
	ll x = mt() % n, y = x, d = 1;
	while (d == 1) {
		x = (mod_prod(x, x, n) + c) % n;
		y = (mod_prod(y, y, n) + c) % n;
		y = (mod_prod(y, y, n) + c) % n;
		d = __gcd(abs(x - y), n);
	}
	return d;
}

template <typename T>
vector<pair<T, int>> factorize(T n) {
	if (n <= 1) return{};
	vector<T> st = { n }, ps;
	while (!st.empty()) {
		T t = st.back(); st.pop_back();
		if (t <= 1) continue;
		if (miller_rabin_test(t)) {
			ps.push_back(t);
			continue;
		}
		T x;
		while (x = pollard_rho(t), x == t);
		st.push_back(x);
		st.push_back(t / x);
	}
	sort(ps.begin(), ps.end());
	vector<pair<T, int>> res;
	for (auto p : ps) {
		if (res.empty() || res.back().first != p) res.emplace_back(p, 1);
		else res.back().second++;
	}
	return res;
}
