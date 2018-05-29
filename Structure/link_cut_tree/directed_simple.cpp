
class lct_node {
	lct_node *l, *r, *p;

	int pos() {
		if (p && p->l == this) return 1;
		if (p && p->r == this) return 3;
		return 0;
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
		while (pos()) {
			int st = pos() ^ p->pos();
			if (!st) p->rot(), rot();
			else if (st == 2) rot(), rot();
			else rot();
		}
	}

public:
	lct_node() : l(nullptr), r(nullptr), p(nullptr) {}
	void expose() {
		for (lct_node *x = this, *y = nullptr; x; y = x, x = x->p) x->splay(), x->r = y;
		splay();
	}
	void link(lct_node *x) {
		expose();
		p = x;
	}
	void cut() {
		expose();
		l->p = nullptr;
		l = nullptr;
	}
	lct_node *lca(lct_node *x) {
		x->expose();
		expose();
		bool same = false;
		lct_node *res = x;
		while (x) {
			if (x->p && !x->pos()) res = x->p;
			if (x == this) same = true;
			x = x->p;
		}
		return same ? res : nullptr;
	}
};
