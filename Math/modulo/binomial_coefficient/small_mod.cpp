
using ll = long long;

template <typename T>
T mod_inv(T a, T md) {
	T b = md, u = 1, v = 0;
	while (b != 0) {
		T t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	u %= md;
	if (u < 0) u += md;
	return u;
}

// initialize:O(md), query:O(log n)
pair<ll, ll> factorial(ll n, ll p, ll md) {
	static unordered_map<ll, vector<int>> sum;
	if (!sum.count(md)) {
		auto& s = sum[md];
		s.assign(md, 1);
		for (int i = 1; i < md; i++) {
			s[i] = i % p ? ((ll)s[i - 1] * i) % md : s[i - 1];
		}
	}
	const auto& s = sum[md];
	ll cnt = 0, res = 1;
	while (n > 1) {
		if (res != 0 && ((n / md) & 1)) res = md - res;
		(res *= s[n % md]) %= md;
		n /= p;
		cnt += n;
	}
	return make_pair(cnt, res);
}

ll nCr(ll n, ll r, ll p, int k = 1) {// nCr mod p^k
	if (r < 0 || n < r) return 0;
	ll md = 1;
	for (int i = 0; i < k; i++) md *= p;
	auto a = factorial(n, p, md);
	auto b = factorial(r, p, md);
	auto c = factorial(n - r, p, md);
	ll res = a.second;
	(res *= mod_inv(b.second, md)) %= md;
	(res *= mod_inv(c.second, md)) %= md;
	int cnt = min(a.first - b.first - c.first, (ll)k);
	for (int i = 0; i < cnt; i++) (res *= p) %= md;
	return res;
}
