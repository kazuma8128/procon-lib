
using ull = unsigned long long;

const int rh_mod = 1e9 + 7;
const int rh_b1 = 1009, rh_b2 = 10007;

class rolling_hash {
	const int n;
	vector<int> r1, r2, h1, h2;
public:
	rolling_hash(const string& s)
		: n(s.size()), r1(n + 1), r2(n + 1), h1(n + 1), h2(n + 1) {
		r1[0] = r2[0] = 1;
		for (int i = 0; i < n; i++) {
			r1[i + 1] = (ull)r1[i] * rh_b1 % rh_mod;
			r2[i + 1] = (ull)r2[i] * rh_b2 % rh_mod;
			h1[i + 1] = ((ull)h1[i] * rh_b1 + s[i]) % rh_mod;
			h2[i + 1] = ((ull)h2[i] * rh_b2 + s[i]) % rh_mod;
		}
	}
	ull get(int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		ull x1 = (h1[r] - (ull)h1[l] * r1[r - l] % rh_mod + rh_mod) % rh_mod;
		ull x2 = (h2[r] - (ull)h2[l] * r2[r - l] % rh_mod + rh_mod) % rh_mod;
		return (x1 << 32LL) | x2;
	}
};
