
// Dynamic Convex Hull Trick　(単調性不要)
// Li Chao Segment Tree ver
// 最初にクエリの値を全列挙できる状況でのみ使える

template <typename T, const T id>
class convex_hull_trick {
	struct line {
		T a, b;
		line(T a_ = 0, T b_ = 0) : a(a_), b(b_) {}
		T get(T x) { return a * x + b; }
	};
	struct node {
		line l;
		node *lch, *rch;
		node(line l_) : l(l_), lch(nullptr), rch(nullptr) {}
	};
	const int n;
	const vector<T> pos;
	node *root;
	node* modify(node *p, int lb, int ub, line& l) {
		if (!p) return new node(l);
		if (p->l.get(pos[lb]) >= l.get(pos[lb]) && p->l.get(pos[ub]) >= l.get(pos[ub])) return p;
		if (p->l.get(pos[lb]) <= l.get(pos[lb]) && p->l.get(pos[ub]) <= l.get(pos[ub])) {
			p->l = l;
			return p;
		}
		int c = (lb + ub) / 2;
		if (p->l.get(pos[c]) < l.get(pos[c])) swap(p->l, l);
		if (p->l.get(pos[lb]) <= l.get(pos[lb]))
			p->lch = modify(p->lch, lb, c, l);
		else
			p->rch = modify(p->rch, c + 1, ub, l);
		return p;
	}
	T sub(node *p, int lb, int ub, int t) const {
		if (!p) return id;
		if (ub - lb == 0) return p->l.get(pos[t]);
		int c = (lb + ub) / 2;
		if (t <= c) return max(p->l.get(pos[t]), sub(p->lch, lb, c, t));
		return max(p->l.get(pos[t]), sub(p->rch, c + 1, ub, t));
	}
public:
	convex_hull_trick(const vector<T>& pos_) : n(pos_.size()), pos(pos_), root(nullptr) {}
	void insert(T a, T b) {
		line l(a, b);
		root = modify(root, 0, n - 1, l);
	}
	T get(T x) const {
		int t = lower_bound(pos.begin(), pos.end(), x) - pos.begin();
		assert(t < n && pos[t] == x);
		return sub(root, 0, n - 1, t);
	}
};

// Dynamic Convex Hull Trick (単調性不要)
// multiset override ver
// 一般的な状況で使えるが、平衡二分木なので少し遅い
// a * b などのオーバーフローに注意

using T = long long;
const T id = -(1ll << 62);

struct Line {
	T m, b;
	mutable function<const Line*()> succ;
	bool operator<(const Line& rhs) const {
		if (rhs.b != id) return m < rhs.m;
		const Line* s = succ();
		if (!s) return false;
		T x = rhs.m;
		return b - s->b < (s->m - m) * x;
	}
};

class convex_hull_trick : public multiset<Line> {
	bool bad(iterator y) {
		auto z = next(y);
		if (y == begin()) {
			if (z == end()) return 0;
			return y->m == z->m && y->b <= z->b;
		}
		auto x = prev(y);
		if (z == end()) return y->m == x->m && y->b <= x->b;
		return (x->b - y->b) * (z->m - y->m) >= (y->b - z->b) * (y->m - x->m);
	}
public:
	void insert_line(T m, T b) {
		auto y = insert({ m, b });
		y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
		if (bad(y)) { erase(y); return; }
		while (next(y) != end() && bad(next(y))) erase(next(y));
		while (y != begin() && bad(prev(y))) erase(prev(y));
	}
	T get(T x) {
		auto l = *lower_bound((Line) { x, id });
		return l.m * x + l.b;
	}
};


// ちょっと早くした Li Chao Segment Tree ver
template <typename T, const T id>
class convex_hull_trick {
	struct line {
		T a, b;
		line(T a_ = 0, T b_ = 0) : a(a_), b(b_) {}
		T get(T x) const { return a * x + b; }
	};
	const int n;
	const vector<T> pos;
	vector<line> data;
	void modify(int p, int lb, int ub, line& l) {
		if (data[p].a == id) {
			data[p] = l;
			return;
		}
		if (data[p].get(pos[lb]) >= l.get(pos[lb]) && data[p].get(pos[ub]) >= l.get(pos[ub])) return;
		if (data[p].get(pos[lb]) <= l.get(pos[lb]) && data[p].get(pos[ub]) <= l.get(pos[ub])) {
			data[p] = l;
			return;
		}
		int c = (lb + ub) >> 1;
		if (data[p].get(pos[c]) < l.get(pos[c])) swap(data[p], l);
		if (data[p].get(pos[lb]) <= l.get(pos[lb]))
			modify(p << 1, lb, c, l);
		else
			modify((p << 1) | 1, c + 1, ub, l);
	}
	T sub(int p, int lb, int ub, int t) const {
		if (data[p].a == id) return id;
		if (ub - lb == 0) return data[p].get(pos[t]);
		int c = (lb + ub) >> 1;
		if (t <= c) return max(data[p].get(pos[t]), sub(p << 1, lb, c, t));
		return max(data[p].get(pos[t]), sub((p << 1) | 1, c + 1, ub, t));
	}
public:
	convex_hull_trick(const vector<T>& pos_) : n(pos_.size()), pos(pos_), data(n << 2, line(id, id)) {}
	void insert(T a, T b) {
		line l(a, b);
		modify(1, 0, n - 1, l);
	}
	T get(T x) const {
		int t = lower_bound(pos.begin(), pos.end(), x) - pos.begin();
		assert(t < n && pos[t] == x);
		return sub(1, 0, n - 1, t);
	}
};
