
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

// initialize:O(n), query:O(1)
ll nCr(int n, int r, ll p, int k) {// nCr mod p^k
	struct data { vector<int> cnt, fac, rfac; };
	static unordered_map<int, data> dat;
	if (r < 0 || n < r) return 0;
	ll md = 1;
	for (int i = 0; i < k; i++) md *= p;
	auto& v = dat[md];
	if ((int)v.fac.size() <= n) {
		if (v.fac.empty()) {
			v.cnt.push_back(0);
			v.fac.push_back(1);
			v.rfac.push_back(1);
		}
		int it = v.fac.size();
		v.cnt.resize(n + 1);
		v.fac.resize(n + 1);
		v.rfac.resize(n + 1);
		for (int i = it; i <= n; i++) {
			int val = i, c = 0;
			while (val % p == 0) val /= p, ++c;
			v.fac[i] = (ll)v.fac[i - 1] * val % md;
			v.cnt[i] = v.cnt[i - 1] + c;
		}
		v.rfac[n] = mod_inv((ll)v.fac[n], md);
		for (int i = n - 1; i >= it; i--) {
			int val = i + 1;
			while (val % p == 0) val /= p;
			v.rfac[i] = (ll)v.rfac[i + 1] * val % md;
		}
	}
	ll res = v.fac[n];
	(res *= v.rfac[r]) %= md;
	(res *= v.rfac[n - r]) %= md;
	int x = v.cnt[n] - v.cnt[r] - v.cnt[n - r];
	if (x >= k) return 0;
	for (int i = 0; i < x; i++) (res *= p) %= md;
	return res;
}
