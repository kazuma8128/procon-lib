
using ll = long long;

const int MAX = 1 << 19;

const ll id = LLONG_MIN;

vector<int> pos;

struct line {
	int a, b;
	line(int a_ = 0, int b_ = 0) : a(a_), b(b_) {}
	ll get(int x) { return (ll)a * x + b; }
};

struct node {
	line l;
	node *lch, *rch;
	node(line l_) : l(l_), lch(nullptr), rch(nullptr) {}
};

node* insert_line(node *p, line& l, int lb, int ub) {
	if (!p) return new node(l);
	if (p->l.get(pos[lb]) >= l.get(pos[lb]) && p->l.get(pos[ub - 1]) >= l.get(pos[ub - 1])) return p;
	p = new node(*p);
	if (p->l.get(pos[lb]) <= l.get(pos[lb]) && p->l.get(pos[ub - 1]) <= l.get(pos[ub - 1])) {
		p->l = l;
		return p;
	}
	int c = (lb + ub) / 2;
	if (p->l.get(pos[c]) < l.get(pos[c])) swap(p->l, l);
	if (p->l.get(pos[lb]) <= l.get(pos[lb]))
		p->lch = insert_line(p->lch, l, lb, c);
	else
		p->rch = insert_line(p->rch, l, c, ub);
	return p;
}

ll sub(node *p, int t, int lb, int ub) {
	if (!p) return id;
	if (ub - lb == 1) return p->l.get(pos[t]);
	int c = (lb + ub) / 2;
	if (t < c) return max(p->l.get(pos[t]), sub(p->lch, t, lb, c));
	return max(p->l.get(pos[t]), sub(p->rch, t, c, ub));
}

ll get_max(node *p, int x) {
	int t = lower_bound(pos.begin(), pos.end(), x) - pos.begin();
	assert(t < (int)pos.size() && pos[t] == x);
	return sub(p, t, 0, pos.size());
}
