
ll prod(ll a, ll b, ll md) {
	if (b < 0) a = -a, b = -b;
	ll res = 0;
	while (b) {
		if (b & 1) {
			res = (res + a) % md;
		}
		a = (a + a) % md;
		b >>= 1;
	}
	return res;
}
