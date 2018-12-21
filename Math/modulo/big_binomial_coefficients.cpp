
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
pair<ll, ll> fast_factorial(ll n, ll p, ll md) {
	static unordered_map<ll, vector<int>> sum;
	if (!sum.count(md)) {
		sum[md].assign(md, 1);
		for (int i = 1; i < md; i++) {
			if (i % p == 0) sum[md][i] = sum[md][i - 1];
			else sum[md][i] = ((ll)sum[md][i - 1] * i) % md;
		}
	}
	ll cnt = 0, res = 1;
	while (n > 1) {
		if (res != 0 && ((n / md) & 1)) res = md - res;
		(res *= sum[md][n % md]) %= md;
		n /= p;
		cnt += n;
	}
	return make_pair(cnt, res);
}

ll fast_nCr(ll n, ll r, ll p, int k = 1) {// nCr mod p^k
	if (r < 0 || n < r) return 0;
	ll md = 1;
	for (int i = 0; i < k; i++) md *= p;
	auto a = fast_factorial(n, p, md);
	auto b = fast_factorial(r, p, md);
	auto c = fast_factorial(n - r, p, md);
	ll res = a.second;
	(res *= mod_inv(b.second, md)) %= md;
	(res *= mod_inv(c.second, md)) %= md;
	int cnt = min(a.first - b.first - c.first, (ll)k);
	for (int i = 0; i < cnt; i++) (res *= p) %= md;
	return res;
}
