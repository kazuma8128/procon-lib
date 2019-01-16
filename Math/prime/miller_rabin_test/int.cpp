
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
