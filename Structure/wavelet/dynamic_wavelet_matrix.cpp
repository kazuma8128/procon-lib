
using COL = bool;
const COL RED = false, BLACK = true;

class fid {

	struct node {
		COL color;
		int size;
		bool val;
		int all;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(0), all(0), p(nullptr), ch{ nullptr, nullptr } {}
		node(COL c, bool v, node *par, node *l, node *r) : color(c), size(1), val(v), all(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(bool val) const {
		return new node(RED, val, NIL, NIL, NIL);
	}
	void update(node *x) {
		x->size = x->ch[0]->size + 1 + x->ch[1]->size;
		x->all = x->ch[0]->all + x->val + x->ch[1]->all;
	}
	void update_up(node *x) {
		while (x != NIL) update(x), x = x->p;
	}
	void rotate(node *x, int b) {
		node *y = x->ch[1 - b];
		x->ch[1 - b] = y->ch[b];
		if (y->ch[b] != NIL) {
			y->ch[b]->p = x;
		}
		y->p = x->p;
		if (x->p == NIL) {
			root = y;
		}
		else {
			x->p->ch[x != x->p->ch[0]] = y;
		}
		y->ch[b] = x;
		x->p = y;
		update(x);
		update(y);
	}
	void insert_fix(node *x) {
		while (x->p->color == RED) {
			int b = (x->p != x->p->p->ch[0]);
			node *y = x->p->p->ch[1 - b];
			if (y->color == RED) {
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
				continue;
			}
			if (x == x->p->ch[1 - b]) {
				x = x->p;
				rotate(x, b);
			}
			x->p->color = BLACK;
			x->p->p->color = RED;
			rotate(x->p->p, 1 - b);
		}
		root->color = BLACK;
	}
	void transplant(node *u, node *v) {
		if (u->p == NIL) {
			root = v;
		}
		else {
			u->p->ch[u != u->p->ch[0]] = v;
		}
		v->p = u->p;
	}
	void erase_fix(node *x) {
		while (x != root && x->color == BLACK) {
			int b = (x != x->p->ch[0]);
			node *w = x->p->ch[1 - b];
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				rotate(x->p, b);
				w = x->p->ch[1 - b];
			}
			if (w->ch[b]->color == BLACK && w->ch[1 - b]->color == BLACK) {
				w->color = RED;
				x = x->p;
				continue;
			}
			if (w->ch[1 - b]->color == BLACK) {
				w->ch[b]->color = BLACK;
				w->color = RED;
				rotate(w, 1 - b);
				w = x->p->ch[1 - b];
			}
			w->color = x->p->color;
			x->p->color = BLACK;
			w->ch[1 - b]->color = BLACK;
			rotate(x->p, b);
			x = root;
		}
		x->color = BLACK;
	}
	node* find_first(node *x) const {
		while (x->ch[0] != NIL) x = x->ch[0];
		return x;
	}
	node* find(node *t, int k) const {
		if (k < 0 || t->size <= k) return NIL;
		node *x = t;
		while (x->ch[0]->size != k) {
			if (k < x->ch[0]->size) {
				x = x->ch[0];
			}
			else {
				k -= x->ch[0]->size + 1;
				x = x->ch[1];
			}
		}
		return x;
	}
	int find(node *t, int l, int r) const {
		if (t == NIL || r <= 0 || t->size <= l) return 0;
		if (l <= 0 && t->size <= r) return t->all;
		int c = t->ch[0]->size;
		return find(t->ch[0], l, r) + (l <= c && c < r ? t->val : 0) + find(t->ch[1], l - (c + 1), r - (c + 1));
	}

public:

	fid() : NIL(new node()), root(NIL) {}
	int size() const {
		return root->size;
	}
	void insert(int k, bool b) {
		node *y = NIL, *v = new_node(b);
		if (root == NIL) {
			root = v;
		}
		else if (k == 0) {
			y = find_first(root);
			y->ch[0] = v;
		}
		else {
			y = find(root, k - 1);
			if (y->ch[1] == NIL) {
				y->ch[1] = v;
			}
			else {
				y = find_first(y->ch[1]);
				y->ch[0] = v;
			}
		}
		v->p = y;
		update_up(y);
		insert_fix(v);
	}
	void erase(int k) {
		node *x = find(root, k);
		node *y = x, *z;
		COL c = y->color;
		if (x->ch[0] == NIL) {
			z = x->ch[1];
			transplant(x, x->ch[1]);
		}
		else if (x->ch[1] == NIL) {
			z = x->ch[0];
			transplant(x, x->ch[0]);
		}
		else {
			y = find_first(x->ch[1]);
			c = y->color;
			z = y->ch[1];
			if (y->p == x) {
				z->p = y;
			}
			else {
				transplant(y, y->ch[1]);
				y->ch[1] = x->ch[1];
				y->ch[1]->p = y;
			}
			transplant(x, y);
			y->ch[0] = x->ch[0];
			y->ch[0]->p = y;
			y->color = x->color;
			update(y);
		}
		update_up(z->p);
		if (c == BLACK) erase_fix(z);
	}
	bool find(int k) const {
		return find(root, k)->val;
	}
	int rank(int k, bool b) const {
		return b ? find(root, 0, k) : k - find(root, 0, k);
	}
	int rank(int l, int r, bool b) const {
		return b ? find(root, l, r) : r - l - find(root, l, r);
	}
	int select(int k, bool b) const {
		int res = 0;
		node *x = root;
		while (true) {
			assert(x != NIL);
			int c = b ? x->ch[0]->all : x->ch[0]->size - x->ch[0]->all;
			if (k == c && x->val == b) return res + x->ch[0]->size;
			if (k < c) {
				x = x->ch[0];
			}
			else {
				k -= c + (x->val == b);
				res += x->ch[0]->size + 1;
				x = x->ch[1];
			}
		}
	}
	int select(int l, int k, bool b) const {
		return select(k + rank(l, b), b);
	}
};

template <typename T>
class red_black_tree {

	struct node {
		COL color;
		int size;
		T val;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(), p(nullptr), ch{ nullptr, nullptr } {}
		node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), val(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(T val) const {
		return new node(RED, val, NIL, NIL, NIL);
	}
	void update(node *x) {
		x->size = x->ch[0]->size + 1 + x->ch[1]->size;
	}
	void update_up(node *x) {
		while (x != NIL) update(x), x = x->p;
	}
	void rotate(node *x, int b) {
		node *y = x->ch[1 - b];
		x->ch[1 - b] = y->ch[b];
		if (y->ch[b] != NIL) {
			y->ch[b]->p = x;
		}
		y->p = x->p;
		if (x->p == NIL) {
			root = y;
		}
		else {
			x->p->ch[x != x->p->ch[0]] = y;
		}
		y->ch[b] = x;
		x->p = y;
		update(x);
		update(y);
	}
	void insert_fix(node *x) {
		while (x->p->color == RED) {
			int b = (x->p != x->p->p->ch[0]);
			node *y = x->p->p->ch[1 - b];
			if (y->color == RED) {
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
				continue;
			}
			if (x == x->p->ch[1 - b]) {
				x = x->p;
				rotate(x, b);
			}
			x->p->color = BLACK;
			x->p->p->color = RED;
			rotate(x->p->p, 1 - b);
		}
		root->color = BLACK;
	}
	void transplant(node *u, node *v) {
		if (u->p == NIL) {
			root = v;
		}
		else {
			u->p->ch[u != u->p->ch[0]] = v;
		}
		v->p = u->p;
	}
	void erase_fix(node *x) {
		while (x != root && x->color == BLACK) {
			int b = (x != x->p->ch[0]);
			node *w = x->p->ch[1 - b];
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				rotate(x->p, b);
				w = x->p->ch[1 - b];
			}
			if (w->ch[b]->color == BLACK && w->ch[1 - b]->color == BLACK) {
				w->color = RED;
				x = x->p;
				continue;
			}
			if (w->ch[1 - b]->color == BLACK) {
				w->ch[b]->color = BLACK;
				w->color = RED;
				rotate(w, 1 - b);
				w = x->p->ch[1 - b];
			}
			w->color = x->p->color;
			x->p->color = BLACK;
			w->ch[1 - b]->color = BLACK;
			rotate(x->p, b);
			x = root;
		}
		x->color = BLACK;
	}
	node* find_first(node *x) const {
		if (x == NIL) return NIL;
		while (x->ch[0] != NIL) x = x->ch[0];
		return x;
	}
	node* find(node *t, int k) const {
		if (k < 0 || t->size <= k) return NIL;
		node *x = t;
		while (x->ch[0]->size != k) {
			if (k < x->ch[0]->size) {
				x = x->ch[0];
			}
			else {
				k -= x->ch[0]->size + 1;
				x = x->ch[1];
			}
		}
		return x;
	}

public:

	red_black_tree() : NIL(new node()), root(NIL) {}
	T find(int k) const {
		return find(root, k)->val;
	}
	void update(int k, T val) {
		node *t = find(root, k);
		t->val = val;
	}
	void insert(int k, T val) {
		node *y = NIL, *v = new_node(val);
		if (root == NIL) {
			root = v;
		}
		else if (k == 0) {
			y = find_first(root);
			y->ch[0] = v;
		}
		else {
			y = find(root, k - 1);
			if (y->ch[1] == NIL) {
				y->ch[1] = v;
			}
			else {
				y = find_first(y->ch[1]);
				y->ch[0] = v;
			}
		}
		v->p = y;
		update_up(y);
		insert_fix(v);
	}
	void erase(int k) {
		node *x = find(root, k);
		node *y = x, *z;
		COL c = y->color;
		if (x->ch[0] == NIL) {
			z = x->ch[1];
			transplant(x, x->ch[1]);
		}
		else if (x->ch[1] == NIL) {
			z = x->ch[0];
			transplant(x, x->ch[0]);
		}
		else {
			y = find_first(x->ch[1]);
			c = y->color;
			z = y->ch[1];
			if (y->p == x) {
				z->p = y;
			}
			else {
				transplant(y, y->ch[1]);
				y->ch[1] = x->ch[1];
				y->ch[1]->p = y;
			}
			transplant(x, y);
			y->ch[0] = x->ch[0];
			y->ch[0]->p = y;
			y->color = x->color;
			update(y);
		}
		update_up(z->p);
		if (c == BLACK) erase_fix(z);
	}

};


template <typename T>
class dynamic_wavelet_matrix {
	T h;
	red_black_tree<T> all;
	vector<fid> data;
	vector<int> mid;

public:
	dynamic_wavelet_matrix(T h_ = 30) : h(h_), data(h), mid(h) {}
	void insert(int k, T val) {
		all.insert(k, val);
		for (T b = 0; b < h; b++) {
			bool d = (val >> (h - 1 - b)) & 1;
			data[b].insert(k, d);
			if (d) {
				k = data[b].rank(k, d) + mid[b];
			}
			else {
				k = data[b].rank(k, d);
				mid[b]++;
			}
		}
	}
	void erase(int k) {
		T val = all.find(k);
		all.erase(k);
		for (T b = 0; b < h; b++) {
			bool d = (val >> (h - 1 - b)) & 1;
			data[b].erase(k);
			if (d) {
				k = data[b].rank(k, d) + mid[b];
			}
			else {
				k = data[b].rank(k, d);
				mid[b]--;
			}
		}
	}
	T get(int k) const {
		return all.find(k);
	}
	int rank(int p, T val) {
		return rank(0, p, val);
	}
	int rank(int l, int r, T val) {
		if (val >> h) return 0;
		for (T b = 0; b < h; b++) {
			if (val & ((T)1 << (h - 1 - b))) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		return r - l;
	}
	int rank_less_than(int l, int r, T ub) {
		if (ub >> h) return r - l;
		int res = 0;
		for (T b = 0; b < h; b++) {
			bool d = (ub >> (h - 1 - b)) & 1;
			int lcnt = data[b].rank(l, d);
			int rcnt = data[b].rank(r, d);
			if (d) res += (r - l) - (rcnt - lcnt);
			l = lcnt;
			r = rcnt;
			if (d) {
				l += mid[b];
				r += mid[b];
			}
		}
		return res;
	}
	int rank_range(int l, int r, T lb, T ub) {
		return rank_less_than(l, r, ub) - rank_less_than(l, r, lb);
	}
	int select(int x, T val) {
		static int left[h];
		int l = 0, r = data[0].size();
		for (T b = 0; b < h; b++) {
			left[b] = l;
			if (val & ((T)1 << (h - 1 - b))) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		for (int b = h - 1; b >= 0; b--) {
			x = data[b].select(left[b], x, (bool)((val >> (h - 1 - b)) & 1)) - left[b];
		}
		return x;
	}
	int select(int l, int r, int x, T val) {
		return select(x + rank(l, val), val);
	}
	T kth_element(int l, int r, int k) const {
		T res = 0;
		for (T b = 0; b < h; b++) {
			int cnt = data[b].rank(l, r, false);
			res <<= 1;
			if (k >= cnt) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
				k -= cnt;
				res |= 1;
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		return res;
	}
};
