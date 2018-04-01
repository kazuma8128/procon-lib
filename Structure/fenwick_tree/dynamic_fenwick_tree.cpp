
using ll = long long;

template <typename T>
class dynamic_fenwick_tree {
	const ll n;
	unordered_map<ll, T> data;
public:
	dynamic_fenwick_tree(ll n_, int pmax = 1e5) : n(n_), data(pmax) {}
	T find(ll p) {
		T res = 0;
		while (p >= 0) {
			res += data[p];
			p = (p & (p + 1)) - 1;
		}
		return res;
	}
	T find(ll l, ll r) {
		return find(r) - find(l - 1);
	}
	void add(ll p, T val) {
		while (p < n) {
			data[p] += val;
			p |= p + 1;
		}
	}
	void update(ll p, T val) {
		add(p, val - find(p));
	}
};
