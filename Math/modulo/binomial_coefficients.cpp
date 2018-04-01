
template<int MOD>
struct mod_int {
	static const int mod = MOD;
	unsigned x;
	mod_int() : x(0) { }
	mod_int(int sig) { int sigt = sig % MOD; if (sigt < 0) sigt += MOD; x = sigt; }
	mod_int(long long sig) { int sigt = sig % MOD; if (sigt < 0) sigt += MOD; x = sigt; }
	int get() const { return (int)x; }

	mod_int &operator+=(mod_int that) { if ((x += that.x) >= MOD) x -= MOD; return *this; }
	mod_int &operator-=(mod_int that) { if ((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
	mod_int &operator*=(mod_int that) { x = (unsigned long long)x * that.x % MOD; return *this; }
	mod_int &operator/=(mod_int that) { return *this *= that.inverse(); }

	mod_int operator+(mod_int that) const { return mod_int(*this) += that; }
	mod_int operator-(mod_int that) const { return mod_int(*this) -= that; }
	mod_int operator*(mod_int that) const { return mod_int(*this) *= that; }
	mod_int operator/(mod_int that) const { return mod_int(*this) /= that; }

	mod_int inverse() const {
		long long a = x, b = MOD, u = 1, v = 0;
		while (b) {
			long long t = a / b;
			a -= t * b; swap(a, b);
			u -= t * v; swap(u, v);
		}
		return mod_int(u);
	}
};

using mint = mod_int<1000000007>;

const int MAX = 2e6;

mint fac[MAX + 1];
mint rfac[MAX + 1];

void init() {
	fac[0] = 1;
	for (int i = 1; i <= MAX; i++) {
		fac[i] = fac[i - 1] * i;
	}
	rfac[MAX] = fac[MAX].inverse();
	for (int i = MAX; i >= 1; i--) {
		rfac[i - 1] = rfac[i] * i;
	}
}

mint nPr(int n, int r) {
	return r < 0 || n < r ? 0 : fac[n] * rfac[n - r];
}

mint nCr(int n, int r) {
	return r < 0 || n < r ? 0 : fac[n] * rfac[n - r] * rfac[r];
}

mint nHr(int n, int r) {
	return r == 0 ? 1 : nCr(n + r - 1, r);
}
