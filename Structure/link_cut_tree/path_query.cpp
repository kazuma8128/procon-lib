
template <typename M> // M : monoid
class lct_node {
	using T = typename M::type;
	lct_node *l, *r, *p;
	bool rev;
	T val, all;

	int pos() {
		if (p && p->l == this) return 1;
		if (p && p->r == this) return 3;
		return 0;
	}
	void update() {
		all = M::op(l ? l->all : M::id(), M::op(val, r ? r->all : M::id()));
	}
	void push() {
		if (pos()) p->push();
		if (rev) {
			swap(l, r);
			if (l) l->rev ^= true;
			if (r) r->rev ^= true;
			rev = false;
		}
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
			int st = pos() ^ p->pos();
			if (!st) p->rot(), rot();
			else if (st == 2) rot(), rot();
			else rot();
		}
	}

public:
	lct_node(int v = 0) : l(nullptr), r(nullptr), p(nullptr), rev(false), val(v), all(v) {}
	void expose() {
		for (lct_node *x = this, *y = nullptr; x; y = x, x = x->p) x->splay(), x->r = y, x->update();
		splay();
	}
	void evert() {
		expose();
		rev = true;
	}
	void link(lct_node *x) {
		evert();
		p = x;
	}
	void cut() {
		expose();
		l->p = nullptr;
		l = nullptr;
		update();
	}
	T find() {
		expose();
		return all;
	}
	void update(T v) {
		expose();
		val = v;
		update();
	}
};

// example of monoid
struct RMQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};
