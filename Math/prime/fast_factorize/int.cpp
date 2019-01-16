
using ll = long long;

template <typename T>
T mod_pow(T x, ll k, T m) {
	T res = 1;
	while (k) {
		if (k & 1) res = res * x % m;
		k >>= 1;
		x = x * x % m;
	}
	return res;
}

bool miller_rabin_test(ll n) {
	const static vector<ll> sprp = { 2, 7, 61 };
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
		x = (x * x + c) % n;
		y = (y * y + c) % n;
		y = (y * y + c) % n;
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
