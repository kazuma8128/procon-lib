
// range update
struct RU {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return INT_MAX; }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return min(l, r); }
	static t1 op2(const t1& l, const t2& r) { return r == id2() ? l : r; }
	static t2 op3(const t2& l, const t2& r) { return r == id2() ? l : r; }
};

// range add
struct RA {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return 0; }
	static t2 id2() { return 0; }
	static t1 op1(const t1& l, const t1& r) { return l + r; }
	static t1 op2(const t1& l, const t2& r) { return l + r; }
	static t2 op3(const t2& l, const t2& r) { return l + r; }
};

// range update and range minimum
struct RURM {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return INT_MAX; }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return min(l, r); }
	static t1 op2(const t1& l, const t2& r) { return r != id2() ? r : l; }
	static t2 op3(const t2& l, const t2& r) { return r != id2() ? r : l; }
};

// range update and range sum
struct RURS {
	using ll = long long;
	struct t1 {
		ll val, size;
		t1(ll v, ll s = 1) : val(v), size(s) {}
	};
	using t2 = ll;
	static t1 id1() { return t1(0, 0); }
	static t2 id2() { return -1; }
	static t1 op1(const t1& l, const t1& r) { return t1(l.val + r.val, l.size + r.size); }
	static t1 op2(const t1& l, const t2& r) { return t1(l.size * r, l.size); }
	static t2 op3(const t2& l, const t2& r) { return r != id2() ? r : l; }
};

// range add and range sum
struct RARS {
	using ll = long long;
	struct t1 {
		ll val, size;
		t1(ll v, ll s = 1) : val(v), size(s) {}
	};
	using t2 = ll;
	static t1 id1() { return t1(0, 0); }
	static t2 id2() { return 0; }
	static t1 op1(const t1& l, const t1& r) { return t1(l.val + r.val, l.size + r.size); }
	static t1 op2(const t1& l, const t2& r) { return t1(l.val + l.size * r, l.size); }
	static t2 op3(const t2& l, const t2& r) { return l + r; }
};

// range add and range minimum
struct RARM {
	using t1 = int;
	using t2 = int;
	static t1 id1() { return 0; }
	static t2 id2() { return 0; }
	static t1 op1(const t1& l, const t1& r) { return max(l, r); }
	static t1 op2(const t1& l, const t2& r) { return l + r; }
	static t2 op3(const t2& l, const t2& r) { return l + r; }
};

// range add and range minimum sum of continuous subsequence 
struct RURMS {
	using ll = long long;
	static const ll INF = 1e17;
	struct t1 {
		ll left, right, all, sum, size;
		t1() : left(-INF), right(-INF), all(-INF), sum(0), size(0) {}
		t1(ll val) : left(val), right(val), all(val), sum(val), size(1) {}
		t1(ll l, ll r, ll a, ll s, ll si) : left(l), right(r), all(a), sum(s), size(si) {}
		bool operator==(const t1& r) const {
			return left == r.left && right == r.right && all == r.all && sum == r.sum && size == r.size;
		}
	};
	using t2 = ll;
	static t1 id1() { return t1(); }
	static t2 id2() { return -INF; }
	static t1 op1(const t1& l, const t1 & r) {
		return t1(max(l.left, l.sum + r.left), max(r.right, r.sum + l.right), max({ l.all, r.all, l.right + r.left }), l.sum + r.sum, l.size + r.size);
	}
	static t1 op2(const t1& l, const t2 & r) {
		if (r == id2()) return l;
		if (r > 0) return t1(r * l.size, r * l.size, r * l.size, r * l.size, l.size);
		return t1(r, r, r, r * l.size, l.size);
	}
	static t2 op3(const t2& l, const t2 & r) {
		return r == id2() ? l : r;
	}
};
