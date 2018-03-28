
using ll = long long;

template <typename T>
class DynamicFenwickTree {
	const ll n;
	unordered_map<ll, T> data;
public:
	DynamicFenwickTree(ll n_, int pmax = 1e5) : n(n_), data(pmax) {}
	T find(ll p) {
		T res = 0;
		while (p >= 0) {
			res += data[p];
			p = (p & (p + 1)) - 1;
		}
		return res;
	}
	void add(ll p, T val) {
		while (p < n) {
			data[p] += val;
			p |= p + 1;
		}
	}
};

template <typename T>
class DynamicRARS {
	const ll n;
	DynamicFenwickTree<T> bit0, bit1;
public:
	DynamicRARS(ll n_) : n(n_), bit0(n), bit1(n) {}
	T find(ll p) {
		return bit1.find(p) * (p + 1) + bit0.find(p);
	}
	T find(ll l, ll r) {
		return find(r) - find(l - 1);
	}
	void add(ll l, ll r, T val) {
		bit0.add(l, -val * l);
		bit1.add(l, val);
		bit0.add(r + 1, val * (r + 1));
		bit1.add(r + 1, -val);
	}
};
