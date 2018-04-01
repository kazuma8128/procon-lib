
int mod_pow(int a, int b, int m) {
	int ret = 1;
	while (b) {
		if (b & 1) ret = (ll)ret * a % m;
		a = (ll)a * a % m;
		b >>= 1;
	}
	return ret;
}

bool is_square(int a, int p) {
	return mod_pow(a, (p - 1) >> 1, p) == 1;
}

int mod_sqrt(int a, int p) {
	if (a == 0) return 0;
	if (p == 2) return a;
	if (!is_square(a, p)) return -1;
	int b = 2;
	while (is_square(((ll)b * b - a + p) % p, p)) b++;
	int w = ((ll)b * b - a + p) % p;

	auto mul = [&](pair<int, int> u, pair<int, int> v) {
		int a = ((ll)u.first * v.first + (ll)u.second * v.second % p * w) % p;
		int b = ((ll)u.first * v.second + (ll)u.second * v.first) % p;
		return make_pair(a, b);
	};

	int e = (p + 1) / 2;
	auto ret = make_pair(1, 0);
	auto v = make_pair(b, 1);
	while (e) {
		if (e & 1) ret = mul(ret, v);
		v = mul(v, v);
		e >>= 1;
	}
	return ret.first;
}


// solve quadratic equation in mod
using ll = long long;

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

int mod;

pair<int, int> solve_quadratic_equation(ll a, ll b, ll c) {
	a = a * 2 % mod;
	c = c * 2 % mod;
	int rt = mod_sqrt((b * b % mod + mod - a * c % mod) % mod, mod);
	if (rt == -1) return make_pair(-1, -1);
	pair<int, int> res(rt, (mod - rt) % mod);
	res.first = (res.first + mod - b) % mod * mod_inv(a, mod) % mod;
	res.second = (res.second + mod - b) % mod * mod_inv(a, mod) % mod;
	if (res.first > res.second) swap(res.first, res.second);
	return res;
}
