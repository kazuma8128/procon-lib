
class fenwick_tree_set {
	const int n;
	int cnt;
	vector<int> data;
	int find(int p) const {
		int res = 0;
		while (p > 0) {
			res += data[p];
			p -= p & -p;
		}
		return res;
	}
	void add(int p, int x) {
		++p;
		while (p <= n) {
			data[p] += x;
			p += p & -p;
		}
	}
public:
	fenwick_tree_set(int maxi)
		: n(maxi + 1), cnt(0), data(n + 1) {}
	int size() const {
		return cnt;
	}
	int count(int val) const {
		assert(0 <= val && val < n);
		return find(val + 1) - find(val);
	}
	void insert(int val) {
		assert(0 <= val && val < n);
		add(val, 1);
		cnt += 1;
	}
	void erase(int val) {
		assert(0 <= val && val < n);
		assert(0 < count(val));
		add(val, -1);
		cnt -= 1;
	}
	int kth_element(int k) const {
		assert(0 <= k && k < cnt);
		int p = 1 << (32 - __builtin_clz(n)), res = 0;
		while (p >>= 1) {
			if (res + p <= n && data[res + p] <= k) {
				k -= data[res + p];
				res += p;
			}
		}
		return res;
	}
};
