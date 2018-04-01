
ll euler_totient(ll n) {
	ll res = n;
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			res = res / i * (i - 1);
			while (n /= i, n % i == 0);
		}
	}
	if (n != 1) res = res / n * (n - 1);
	return res;
}
