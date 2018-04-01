
template <typename T>
class red_black_tree {

	using COL = bool;
	static const COL RED = false, BLACK = true;

	struct node {
		COL color;
		int size;
		T val;
		node *p, *ch[2];
		node() : color(BLACK), size(0), val(), p(nullptr), ch{ nullptr, nullptr } {}
		node(COL c, T v, node *par, node *l, node *r) : color(c), size(1), val(v), p(par), ch{ l, r } {}
	} *NIL, *root;

	node* new_node(T val) {
		return new node(RED, val, NIL, NIL, NIL);
	}
	void update(node *x) {
		x->size = x->ch[0]->size + 1 + x->ch[1]->size;
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
	node* find(node *x, T val) const {
		while (x != NIL) {
			if (val == x->val) return x;
			x = x->ch[val > x->val];
		}
		return NIL;
	}
	node* minimum(node *x) const {
		if (x == NIL) return NIL;
		while (x->ch[0] != NIL) {
			x = x->ch[0];
		}
		return x;
	}
	node* maximum(node *x) const {
		if (x == NIL) return NIL;
		while (x->ch[1] != NIL) {
			x = x->ch[1];
		}
		return x;
	}
public:
	red_black_tree() : NIL(new node()), root(NIL) {}
	int size() const {
		return root->size;
	}
	T minimum() const {
		return minimum(root)->val;
	}
	T maximum() const {
		return maximum(root)->val;
	}
	bool count(T val) const {
		return find(root, val) != NIL;
	}
	T operator[](int k) const {
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
		return x->val;
	}
	void insert(T val) {
		node *x = root, *y = NIL;
		while (x != NIL) {
			if (val == x->val) return;
			y = x;
			x = x->ch[val > x->val];
		}
		node *v = new_node(val);
		v->p = y;
		if (y == NIL) {
			root = v;
		}
		else {
			y->ch[val > y->val] = v;
		}
		while (y != NIL) update(y), y = y->p;
		insert_fix(v);
	}
	void erase(T val) {
		node *x = find(root, val);
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
			y = minimum(x->ch[1]);
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
		node *t = z->p;
		while (t != NIL) update(t), t = t->p;
		if (c == BLACK) {
			erase_fix(z);
		}
	}
};
