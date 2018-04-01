
ll mod_inv(ll a, ll md = 1e9 + 7) {
	ll b = md, u = 1, v = 0;
	while (b) {
		ll t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	u %= md;
	if (u < 0) u += md;
	return u;
}
