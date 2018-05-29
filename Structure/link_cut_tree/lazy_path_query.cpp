
template <typename M>
class lct_node {
	using T1 = typename M::t1;
	using T2 = typename M::t2;
	lct_node *l, *r, *p;
	bool rev;
	T1 val, all;
	T2 lazy;

	int pos() {
		if (p && p->l == this) return -1;
		if (p && p->r == this) return 1;
		return 0;
	}
	void update() {
		T1 left = l ? M::op2(l->all, l->lazy) : M::id1();
		T1 right = r ? M::op2(r->all, r->lazy) : M::id1();
		all = M::op1(left, M::op1(val, right));
	}
	void push() {
		if (pos()) p->push();
		if (rev) {
			swap(l, r);
			if (l) l->rev ^= true, M::rev1(l->val), M::rev1(l->all), M::rev2(l->lazy);
			if (r) r->rev ^= true, M::rev1(r->val), M::rev1(r->all), M::rev2(r->lazy);
			rev = false;
		}
		if (lazy == M::id2()) return;
		val = M::op2(val, lazy);
		all = M::op2(all, lazy);
		if (l) l->lazy = M::op3(l->lazy, lazy);
		if (r) r->lazy = M::op3(r->lazy, lazy);
		lazy = M::id2();
	}
	void rot() {
		lct_node *par = p;
		lct_node *mid;
		if (p->l == this) {
			mid = r;
			r = par;
			par->l = mid;
		}
		else {
			mid = l;
			l = par;
			par->r = mid;
		}
		if (mid) mid->p = par;
		p = par->p;
		par->p = this;
		if (p && p->l == par) p->l = this;
		if (p && p->r == par) p->r = this;
		par->update();
		update();
	}
	void splay() {
		push();
		while (pos()) {
			int st = pos() * p->pos();
			if (!st) rot();
			else if (st == 1) p->rot(), rot();
			else rot(), rot();
		}
	}

public:
	lct_node() : l(nullptr), r(nullptr), p(nullptr), rev(false), val(), all(), lazy(M::id2()) {}
	lct_node(const T1& v) : l(nullptr), r(nullptr), p(nullptr), rev(false), val(v), all(v), lazy(M::id2()) {}
	void expose() {
		for (lct_node *x = this, *y = nullptr; x; y = x, x = x->p) x->splay(), x->r = y, x->update();
		splay();
	}
	void link(lct_node *x) {
		x->expose();
		expose();
		p = x;
	}
	void cut() {
		expose();
		l->p = nullptr;
		l = nullptr;
		update();
	}
	void evert() {
		expose();
		rev = true;
		M::rev1(val);
		M::rev1(all);
		M::rev2(lazy);
	}
	T1 find() {
		expose();
		return all;
	}
	void update(T2 v) {
		expose();
		lazy = M::op3(lazy, v);
	}
};

// template of monoid
class monoid {
public:
	class t1;
	class t2;

	static t1 id1();
	static t2 id2();

	static t1 op1(const t1& l, const t1 & r);
	static t1 op2(const t1& l, const t2 & r);
	static t2 op3(const t2& l, const t2 & r);

	static void rev1(t1& x);
	static void rev2(t2& x);
};
