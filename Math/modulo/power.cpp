
ll power(ll a, ll b, ll md) {
	assert(b >= 0);
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % md;
		a = a * a % md;
		b >>= 1;
	}
	return res;
}
