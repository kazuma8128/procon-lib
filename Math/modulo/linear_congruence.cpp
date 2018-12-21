
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

template <typename T>
T linear_congruence(const vector<pair<T, T>>& cs) { // x % B = A
	T x = 0, m = 1;
	for (auto cc : cs) {
		T A, B;
		tie(A, B) = cc;
		T a = m, b = A - x, d = __gcd(B, a);
		if (b % d != 0) return -1;
		a /= d; b /= d; B /= d;
		T t = b % B * mod_inv(a, B) % B;
		if (t < 0) t += B;
		x += t * m;
		m *= B;
	}
	return x;
}
