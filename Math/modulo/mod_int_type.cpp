
// constant mod ver
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

using mint = mod_int<1000000007>;
