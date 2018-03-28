
template<typename T>
class retroactive_array {
	vector<T> data;
	vector<pair<int, T>> hist;
public:
	retroactive_array(int N) : data(N) {}
	retroactive_array(int N, T val) : data(N, val) {}
	retroactive_array(const vector<T>& v) : data(v) {}
	retroactive_array(vector<T>&& v) : data(move(v)) {}
	void change(int k, T val) {
		hist.emplace_back(k, data[k]);
		data[k] = val;
	}
	T operator[](int k) const {
		return data[k];
	}
	int get_version() const {
		return hist.size();
	}
	void rollback(int ver) {
		assert(0 <= ver && ver <= (int)hist.size());
		int cnt = hist.size() - ver;
		while (cnt--) {
			data[hist.back().first] = hist.back().second;
			hist.pop_back();
		}
	}
};
template <typename T, const T id>
class retroactive_CHT {
	struct line {
		T a, b;
		line(T a_ = 0, T b_ = 0) : a(a_), b(b_) {}
		T get(T x) const { return a * x + b; }
	};
	struct node {
		line l;
		node *lch, *rch;
		node(line l_) : l(l_), lch(nullptr), rch(nullptr) {}
	};

	const int n;
	const vector<T> pos;
	retroactive_array<line> data;

	void modify(int p, int lb, int ub, line& l) {
		if (data[p].a == id) {
			data.change(p, l);
			return;
		}
		if (data[p].get(pos[lb]) >= l.get(pos[lb]) && data[p].get(pos[ub - 1]) >= l.get(pos[ub - 1])) return;
		if (data[p].get(pos[lb]) <= l.get(pos[lb]) && data[p].get(pos[ub - 1]) <= l.get(pos[ub - 1])) {
			data.change(p, l);
			return;
		}
		int c = (lb + ub) >> 1;
		if (data[p].get(pos[c]) < l.get(pos[c])) {
			line tmp = data[p];
			data.change(p, l);
			l = tmp;
		}
		if (data[p].get(pos[lb]) <= l.get(pos[lb]))
			modify(p << 1, lb, c, l);
		else
			modify((p << 1) | 1, c, ub, l);
	}
	T sub(int p, int lb, int ub, int t) const {
		if (data[p].a == id) return id;
		if (ub - lb == 1) return data[p].get(pos[t]);
		int c = (lb + ub) >> 1;
		if (t < c) return max(data[p].get(pos[t]), sub(p << 1, lb, c, t));
		return max(data[p].get(pos[t]), sub((p << 1) | 1, c, ub, t));
	}

public:
	retroactive_CHT(const vector<T>& pos_)
		: n(pos_.size()), pos(pos_), data(n << 2, line(id, id)) {}
	void insert(T a, T b) {
		line l(a, b);
		modify(1, 0, n, l);
	}
	T get(T x) const {
		int t = lower_bound(pos.begin(), pos.end(), x) - pos.begin();
		assert(t < n && pos[t] == x);
		return sub(1, 0, n, t);
	}
	int get_version() const {
		return data.get_version();
	}
	void rollback(int ver) {
		data.rollback(ver);
	}
};
