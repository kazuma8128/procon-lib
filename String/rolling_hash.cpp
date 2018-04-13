
using ll = long long;
using pll = pair<ll, ll>;
const ll mod = 1e9 + 7;
const ll b1 = 1009, b2 = 10007;

ll power(ll a, ll b) {
	ll res = 1; a %= mod;
	while (b) {
		if (b & 1) {
			res = (res * a) % mod;
		}
		a = (a * a) % mod;
		b >>= 1;
	}
	return res;
}

class rolling_hash {
	int N;
	string s;
	vector<ll> r1, r2, h1, h2, rh1, rh2;
public:
	rolling_hash(const string& s_)
		: N((int)s_.size()), s(s_), r1(N + 1), r2(N + 1), h1(N + 1), h2(N + 1), rh1(N + 1), rh2(N + 1) {
		r1[0] = r2[0] = 1;
		for (int i = 1; i <= N; i++) r1[i] = r1[i - 1] * b1 % mod;
		for (int i = 1; i <= N; i++) r2[i] = r2[i - 1] * b2 % mod;

		h1[0] = h2[0] = 1;
		for (int i = 1; i <= N; i++) h1[i] = (h1[i - 1] * b1 + s[i - 1]) % mod;
		for (int i = 1; i <= N; i++) h2[i] = (h2[i - 1] * b2 + s[i - 1]) % mod;
	}
	pll get(int l, int r) {
		return pll((h1[r] - h1[l] * r1[r - l] % mod + mod) % mod, (h2[r] - h2[l] * r2[r - l] % mod + mod) % mod);
	}
};

// two-dimensional
using ull = unsigned long long;

bool rolling_hash2(int pll, int Q, const vector<vector<char>>& a, vector<vector<ull>>& res) {
	const ull B1 = 9973;
	const ull B2 = 100000007;
	if ((int)a.size() < pll || (int)a[0].size() < Q) return false;
	vector<vector<ull>> tmp(a.size(), vector<ull>(a[0].size() - Q + 1));
	res = vector<vector<ull>>(a.size() - pll + 1, vector<ull>(a[0].size() - Q + 1));
	ull t1 = 1;
	for (int j = 0; j < Q; j++) t1 *= B1;
	for (int i = 0; i < (int)a.size(); i++) {
		ull e = 0;
		for (int j = 0; j < Q; j++) e = e * B1 + a[i][j];
		for (int j = 0; j + Q <= (int)a[0].size(); j++) {
			tmp[i][j] = e;
			if (j + Q < (int)a[0].size()) e = e * B1 - t1 * a[i][j] + a[i][j + Q];
		}
	}
	ull t2 = 1;
	for (int i = 0; i < pll; i++) t2 *= B2;
	for (int j = 0; j + Q <= (int)a[0].size(); j++) {
		ull e = 0;
		for (int i = 0; i < pll; i++) e = e * B2 + tmp[i][j];
		for (int i = 0; i + pll <= (int)a.size(); i++) {
			res[i][j] = e;
			if (i + pll < (int)a.size()) e = e * B2 - t2 * tmp[i][j] + tmp[i + pll][j];
		}
	}
	return true;
}
