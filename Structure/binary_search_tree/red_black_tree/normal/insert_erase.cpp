
// simple ver
template <typename T>
class RedBlackTree {

	using COL = bool;
	const static COL RED = false, BLACK = true;

	struct node {
		COL color;
		int size;
		T val;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(id), p(nullptr), ch{ nullptr, nullptr } {}
		node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), val(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(T val) {
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
	node* find_first(node *x) {
		if (x == NIL) return NIL;
		while (x->ch[0] != NIL) x = x->ch[0];
		return x;
	}
	node* find(node *t, int k) {
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

	RedBlackTree() : NIL(new node()), root(NIL) {}
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
		erase(x);
	}
	void erase(node *x) {
		if (x == NIL) return;
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


// able to get range value
using T = int;
const T id = INT_MAX;
T op(T l, T r) {
	return min(l, r);
}

class RedBlackTree {

	using COL = bool;
	const static COL RED = false, BLACK = true;

	struct node {
		COL color;
		int size;
		T val, all;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(id), all(id), p(nullptr), ch{ nullptr, nullptr } {}
		node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), val(v), all(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(T val) {
		return new node(RED, val, NIL, NIL, NIL);
	}
	void update(node *x) {
		x->size = x->ch[0]->size + 1 + x->ch[1]->size;
		x->all = op(op(x->ch[0]->all, x->val), x->ch[1]->all);
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
	node* find_first(node *x) {
		if (x == NIL) return NIL;
		while (x->ch[0] != NIL) x = x->ch[0];
		return x;
	}
	node* find(node *t, int k) {
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
	T find(node *t, int l, int r) {
		if (t == NIL || r <= 0 || t->size <= l) return id;
		if (l <= 0 && t->size <= r) return t->all;
		int c = t->ch[0]->size;
		return op(op(find(t->ch[0], l, r), l <= c && c < r ? t->val : id), find(t->ch[1], l - (c + 1), r - (c + 1)));
	}

public:

	RedBlackTree() : NIL(new node()), root(NIL) {}
	node* find(int k) {
		return find(root, k);
	}
	T find(int l, int r) {
		return find(root, l, r + 1);
	}
	void update(int k, T val) {
		node *t = find(root, k);
		t->val = val;
		update_up(t);
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
		erase(x);
	}
	void erase(node *x) {
		if (x == NIL) return;
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
		if (c == BLACK) {
			erase_fix(z);
		}
	}

};

// generalized ver
struct RMQ {
	using type = int;
	static const type id = INT_MAX;
	static type op(type l, type r) { return min(l, r); }
};

template <typename M>
class RedBlackTree {

	using T = typename M::type;
	using COL = bool;
	const static COL RED = false, BLACK = true;

	struct node {
		COL color;
		int size;
		T val, all;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(M::id), all(M::id), p(this), ch{ this, this } {}
		node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), val(v), all(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(T val) {
		return new node(RED, val, NIL, NIL, NIL);
	}
	void update(node *x) {
		x->size = x->ch[0]->size + 1 + x->ch[1]->size;
		x->all = M::op(M::op(x->ch[0]->all, x->val), x->ch[1]->all);
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
	node* find_first(node *x) {
		while (x->ch[0] != NIL) x = x->ch[0];
		return x;
	}
	node* find(node *t, int k) {
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
	T find(node *t, int l, int r) {
		if (t == NIL || r <= 0 || t->size <= l) return M::id;
		if (l <= 0 && t->size <= r) return t->all;
		int c = t->ch[0]->size;
		return M::op(M::op(find(t->ch[0], l, r), l <= c && c < r ? t->val : M::id), find(t->ch[1], l - (c + 1), r - (c + 1)));
	}

public:

	RedBlackTree() : NIL(new node()), root(NIL) {}
	node* find(int k) {
		return find(root, k);
	}
	T find(int l, int r) {
		return find(root, l, r);
	}
	void update(int k, T val) {
		node *t = find(root, k);
		t->val = val;
		update_up(t);
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
		erase(x);
	}
	void erase(node *x) {
		if (x == NIL) return;
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



// function ver
using T = int;
const T id = INT_MAX;
T op(T l, T r) {
	return min(l, r);
}

using COL = bool;
const COL RED = false, BLACK = true;

struct node {
	COL color;
	int size;
	int level;
	T val, all;
	node *p, *ch[2];
	node() : color(BLACK), size(0), level(0), val(id), all(id), p(nullptr), ch{ nullptr, nullptr } {}
	node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), level(0), val(v), all(v), p(par), ch{ l, r } {}
};

node* const NIL = new node();

node* new_node(T val) {
	return new node(RED, val, NIL, NIL, NIL);
}
void update(node *x) {
	x->size = x->ch[0]->size + 1 + x->ch[1]->size;
	x->level = x->ch[0]->level + (x->color == BLACK);
	x->all = op(op(x->ch[0]->all, x->val), x->ch[1]->all);
}
void update(node *root, node *x) {
	while (x != NIL) update(x), x = x->p;
}
void rotate(node *&root, node *x, int b) {
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
void insert_fix(node *&root, node *x) {
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
			rotate(root, x, b);
		}
		x->p->color = BLACK;
		x->p->p->color = RED;
		rotate(root, x->p->p, 1 - b);
	}
	root->color = BLACK;
}
void transplant(node *&root, node *u, node *v) {
	if (u->p == NIL) {
		root = v;
	}
	else {
		u->p->ch[u != u->p->ch[0]] = v;
	}
	v->p = u->p;
}
void erase_fix(node *&root, node *x) {
	while (x != root && x->color == BLACK) {
		int b = (x != x->p->ch[0]);
		node *w = x->p->ch[1 - b];
		if (w->color == RED) {
			w->color = BLACK;
			x->p->color = RED;
			rotate(root, x->p, b);
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
			rotate(root, w, 1 - b);
			w = x->p->ch[1 - b];
		}
		w->color = x->p->color;
		x->p->color = BLACK;
		w->ch[1 - b]->color = BLACK;
		rotate(root, x->p, b);
		x = root;
	}
	x->color = BLACK;
}
node* find_first(node *x) {
	if (x == NIL) return NIL;
	while (x->ch[0] != NIL) x = x->ch[0];
	return x;
}
node* find(node *&root, int k) {
	if (k < 0 || root->size <= k) return NIL;
	node *x = root;
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
void insert(node *&root, int k, T val) {
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
	update(root, y);
	insert_fix(root, v);
}
void erase(node *&root, node *x) {
	if (x == NIL) return;
	node *y = x, *z;
	COL c = y->color;
	if (x->ch[0] == NIL) {
		z = x->ch[1];
		transplant(root, x, x->ch[1]);
	}
	else if (x->ch[1] == NIL) {
		z = x->ch[0];
		transplant(root, x, x->ch[0]);
	}
	else {
		y = find_first(x->ch[1]);
		c = y->color;
		z = y->ch[1];
		if (y->p == x) {
			z->p = y;
		}
		else {
			transplant(root, y, y->ch[1]);
			y->ch[1] = x->ch[1];
			y->ch[1]->p = y;
		}
		transplant(root, x, y);
		y->ch[0] = x->ch[0];
		y->ch[0]->p = y;
		y->color = x->color;
		update(y);
	}
	update(root, z->p);
	if (c == BLACK) {
		erase_fix(root, z);
	}
}
void erase(node *&root, int k) {
	node *x = find(root, k);
	erase(root, x);
}
T find(node *t, int l, int r) {
	if (t == NIL || r <= 0 || t->size <= l) return id;
	if (l <= 0 && t->size <= r) return t->all;
	int c = t->ch[0]->size;
	return op(op(find(t->ch[0], l, r), l <= c && c < r ? t->val : id), find(t->ch[1], l - (c + 1), r - (c + 1)));
}

// not verified merge
node* merge(node *rt1, node *rt2) {
	if (rt1 == NIL) return rt2;
	if (rt2 == NIL) return rt1;
	if (rt1->level >= rt2->level) {
		node *x = rt1;
		while (x->level != rt2->level || x->color != BLACK) {
			x = x->ch[1];
		}
		node *y = new_node(T());
		node *z = x->p;
		transplant(rt1, x, y);
		y->ch[0] = x;
		y->ch[1] = rt2;
		x->p = rt2->p = y;
		update(y);
		while (z != NIL) update(z), z = z->p;
		insert_fix(rt1, y);
		erase(rt1, y);
		return rt1;
	}
	else {
		node *x = rt2;
		while (x->level != rt1->level || x->color != BLACK) {
			x = x->ch[0];
		}
		node *y = new_node(T());
		node *z = x->p;
		transplant(rt2, x, y);
		y->ch[0] = rt1;
		y->ch[1] = x;
		x->p = rt1->p = y;
		update(y);
		while (z != NIL) update(z), z = z->p;
		insert_fix(rt2, y);
		erase(rt2, y);
		return rt2;
	}
}
