
template<int MOD>
struct mod_int {
	static const int Mod = MOD;
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

// not general
template <int MD>
mod_int<MD> lagrange_polynomial(const vector<mod_int<MD>>& ys, long long n) {
	int k = ys.size() - 1;
	if (n <= k) return ys[n];
	n %= MD;
	mod_int<MD> qi = 1, qt = n;
	for (int i = 1; i <= k; i++) {
		qi *= MD - i;
		qt *= n - i;
	}
	mod_int<MD> res = ys[0] / (qi * n);
	for (int i = 1; i <= k; i++) {
		(qi *= i) /= MD - k + (i - 1);
		res += ys[i] / (qi * (n - i));
	}
	return res * qt;
}

const int mod = 1e9 + 7;

using mint = mod_int<mod>;
