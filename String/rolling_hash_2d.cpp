
using ull = unsigned long long;

class rolling_hash_2d {
	static const ull b1 = 9973, b2 = 100000007;
	const int h, w;
	vector<ull> rh, rw;
	vector<vector<ull>> hs;
public:
	template <class T> rolling_hash_2d(const vector<vector<T>>& a)
		: h(a.size()), w(a[0].size()), rh(h + 1), rw(w + 1), hs(h + 1, vector<ull>(w + 1))
	{
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++)
				hs[i + 1][j + 1] = hs[i + 1][j] * b2 + a[i][j];
			for (int j = 0; j < w; j++)
				hs[i + 1][j + 1] += hs[i][j + 1] * b1;
		}
		rh[0] = rw[0] = 1;
		for (int i = 0; i < h; i++) rh[i + 1] = rh[i] * b1;
		for (int i = 0; i < w; i++) rw[i + 1] = rw[i] * b2;
	}
	ull get(int li, int lj, int ri, int rj) {
		assert(0 <= li && li <= ri && ri <= h);
		assert(0 <= lj && lj <= rj && rj <= w);
		ull u = hs[ri][rj] - hs[ri][lj] * rw[rj - lj];
		ull d = hs[li][rj] - hs[li][lj] * rw[rj - lj];
		return u - d * rh[ri - li];
	}
};


// double hash
template <ull mod> class single_rolling_hash_2d {
	static const ull b1 = 9973, b2 = 100000007;
	const int h, w;
	vector<int> rh, rw;
	vector<vector<int>> hs;
public:
	template <class T> single_rolling_hash_2d(const vector<vector<T>>& a)
		: h(a.size()), w(a[0].size()), rh(h + 1), rw(w + 1), hs(h + 1, vector<int>(w + 1))
	{
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++)
				hs[i + 1][j + 1] = ((ull)hs[i + 1][j] * b2 + (ull)a[i][j] % mod) % mod;
			for (int j = 0; j < w; j++)
				hs[i + 1][j + 1] = (hs[i + 1][j + 1] + (ull)hs[i][j + 1] * b1) % mod;
		}
		rh[0] = rw[0] = 1;
		for (int i = 0; i < h; i++) rh[i + 1] = rh[i] * b1 % mod;
		for (int i = 0; i < w; i++) rw[i + 1] = rw[i] * b2 % mod;
	}
	ull get(int li, int lj, int ri, int rj) {
		assert(0 <= li && li <= ri && ri <= h);
		assert(0 <= lj && lj <= rj && rj <= w);
		ull u = hs[ri][rj] + mod - (ull)hs[ri][lj] * rw[rj - lj] % mod; u %= mod;
		ull d = hs[li][rj] + mod - (ull)hs[li][lj] * rw[rj - lj] % mod; (d *= rh[ri - li]) %= mod;
		return u >= d ? u - d : u + mod - d;
	}
};
class rolling_hash_2d {
	static const ull mod1 = 1e9 + 7, mod2 = 1e9 + 9;
	single_rolling_hash_2d<mod1> h1;
	single_rolling_hash_2d<mod2> h2;
public:
	template <class T>
	rolling_hash_2d(const vector<vector<T>>& a) : h1(a), h2(a) {}
	ull get(int li, int lj, int ri, int rj) {
		return (h1.get(li, lj, ri, rj) << 32) | h2.get(li, lj, ri, rj);
	}
};
