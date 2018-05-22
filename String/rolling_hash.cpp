
using ll = long long;
using pii = pair<int, int>;

const ll mod = 1e9 + 7;
const ll b1 = 1009, b2 = 10007;

class rolling_hash {
	const int n;
	string s;
	vector<int> r1, r2, h1, h2;
public:
	rolling_hash(const string& s_) : n((int)s_.size()), s(s_), r1(n + 1), r2(n + 1), h1(n + 1), h2(n + 1) {
		r1[0] = r2[0] = h1[0] = h2[0] = 1;
		for (int i = 1; i <= n; i++) r1[i] = (ll)r1[i - 1] * b1 % mod;
		for (int i = 1; i <= n; i++) r2[i] = (ll)r2[i - 1] * b2 % mod;
		for (int i = 1; i <= n; i++) h1[i] = ((ll)h1[i - 1] * b1 + s[i - 1]) % mod;
		for (int i = 1; i <= n; i++) h2[i] = ((ll)h2[i - 1] * b2 + s[i - 1]) % mod;
	}
	pii get(int l, int r) {
		return pii((h1[r] - (ll)h1[l] * r1[r - l] % mod + mod) % mod, (h2[r] - (ll)h2[l] * r2[r - l] % mod + mod) % mod);
	}
};

// two-dimensional
using ull = unsigned long long;

bool rolling_hash2(int h, int w, const vector<vector<char>>& a, vector<vector<ull>>& res) {
	const ull B1 = 9973;
	const ull B2 = 100000007;
	if ((int)a.size() < h || (int)a[0].size() < w) return false;
	vector<vector<ull>> tmp(a.size(), vector<ull>(a[0].size() - w + 1));
	res = vector<vector<ull>>(a.size() - h + 1, vector<ull>(a[0].size() - w + 1));

	ull t1 = 1;
	for (int j = 0; j < w; j++) t1 *= B1;

	for (int i = 0; i < (int)a.size(); i++) {
		ull e = 0;
		for (int j = 0; j < w; j++) e = e * B1 + a[i][j];

		for (int j = 0; j + w <= (int)a[0].size(); j++) {
			tmp[i][j] = e;
			if (j + w < (int)a[0].size()) e = e * B1 - t1 * a[i][j] + a[i][j + w];
		}
	}

	ull t2 = 1;
	for (int i = 0; i < h; i++) t2 *= B2;

	for (int j = 0; j + w <= (int)a[0].size(); j++) {
		ull e = 0;
		for (int i = 0; i < h; i++) e = e * B2 + tmp[i][j];

		for (int i = 0; i + h <= (int)a.size(); i++) {
			res[i][j] = e;
			if (i + h < (int)a.size()) e = e * B2 - t2 * tmp[i][j] + tmp[i + h][j];
		}
	}
	return true;
}
