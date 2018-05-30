
class lct_node {
	lct_node *l, *r, *p;
	bool rev;

	int pos() {
		if (p && p->l == this) return 1;
		if (p && p->r == this) return 3;
		return 0;
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
	lct_node() : l(nullptr), r(nullptr), p(nullptr), rev(false) {}
	void expose() {
		for (lct_node *x = this, *y = nullptr; x; y = x, x = x->p) x->splay(), x->r = y;
		splay();
	}
	void evert() {
		expose();
		rev = true;
	}
	bool same(lct_node *x) {
		if (x == this) return true;
		evert();
		x->expose();
		return !!p;
	}
	void link(lct_node *x) {
		if (same(x)) return;
		p = x;
	}
	void cut(lct_node *x) {
		x->evert();
		expose();
		if (!l || l != x) return;
		l->p = nullptr;
		l = nullptr;
	}
};
