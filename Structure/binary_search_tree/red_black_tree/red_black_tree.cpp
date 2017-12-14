
// merge/split
using T = int;
const T id = INT_MAX;
T op(T l, T r) {
	return min(l, r);
}

enum COL { BLACK, RED };

struct node;
node *update(node *t);
struct node {
	T val, all;
	node *ch[2];
	COL color;
	int level;
	int size;
	node() {}
	void init(T v) {
		val = v;
		all = v;
		color = BLACK;
		level = 0;
		size = 1;
		ch[0] = ch[1] = nullptr;
	}
	void init(node *l, node *r, COL c) {
		val = id;
		color = c;
		ch[0] = l;
		ch[1] = r;
		update(this);
	}
};

const int pmax = 1e7;
node pool[pmax];
int it = 0;

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id : t->all; }

int ranks(node *t) { return !t ? 0 : t->level; }

node *update(node *t) {
	t->size = count(t->ch[0]) + count(t->ch[1]);
	t->all = op(que(t->ch[0]), op(t->val, que(t->ch[1])));
	t->level = ranks(t->ch[0]) + (t->ch[0]->color == BLACK);
	return t;
}

node *new_leaf(T v) {
	assert(it < pmax);
	pool[it].init(v);
	return &pool[it++];
}

node *new_node(node *l, node *r, COL c) {
	assert(it < pmax);
	pool[it].init(l, r, c);
	return &pool[it++];
}

node *rotate(node *t, int b) {
	node *s = t->ch[1 - b];
	t->ch[1 - b] = s->ch[b];
	s->ch[b] = t;
	update(t); update(s);
	return s;
}

node *submerge(node *l, node *r) {
	if (l->level < r->level) {
		node *c = submerge(l, r->ch[0]);
		r->ch[0] = c;
		if (r->color == BLACK && c->color == RED && c->ch[0] && c->ch[0]->color == RED) {
			if (r->ch[1]->color == BLACK) {
				r->color = RED;
				c->color = BLACK;
				return rotate(r, 1);
			}
			else {
				c->color = BLACK;
				r->ch[1]->color = BLACK;
				r->color = RED;
				return update(r);
			}
		}
		else {
			return update(r);
		}
	}
	else if (l->level > r->level) {
		node *c = submerge(l->ch[1], r);
		l->ch[1] = c;
		if (l->color == BLACK && c->color == RED && c->ch[1] && c->ch[1]->color == RED) {
			if (l->ch[0]->color == BLACK) {
				l->color = RED;
				c->color = BLACK;
				return rotate(l, 0);
			}
			else {
				l->ch[0]->color = BLACK;
				c->color = BLACK;
				l->color = RED;
				return update(l);
			}
		}
		else {
			return update(l);
		}
	}
	else {
		return new_node(l, r, RED);
	}
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	node *c = submerge(l, r);
	c->color = BLACK;
	return c;
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
	if (k == 0) return make_pair(nullptr, t);
	if (k >= count(t)) return make_pair(t, nullptr);
	int c = count(t->ch[0]);
	if (k < c) {
		pair<node*, node*> p = split(t->ch[0], k);
		return make_pair(p.first, merge(p.second, t->ch[1]));
	}
	else if (k > c) {
		pair<node*, node*> p = split(t->ch[1], k - c);
		return make_pair(merge(t->ch[0], p.first), p.second);
	}
	else {
		return make_pair(t->ch[0], t->ch[1]);
	}
}
//--------------------------------------options---------------------------------------------------------

node *insert(node *t, int k, int v) {
	pair<node*, node*> s = split(t, k);
	return merge(merge(s.first, new_leaf(v)), s.second);
}

node *erase(node *t, int k) {
	pair<node*, node*> s1 = split(t, k), s2 = split(s1.second, 1);
	return merge(s1.first, s2.second);
}

T find(node *t, int l, int r) {
	if (!t) return id;
	if (r < 0 || l >= count(t)) return id;
	if (l <= 0 && r >= t->size) return t->all;
	int c = count(t->ch[0]);
	return op(!t->ch[0] ? id : find(t->ch[0], l, r), op(l <= c && c < r ? t->val : id, !t->ch[1] ? id : find(t->ch[1], l - (c + 1), r - (c + 1))));
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->ch[0]);
	return c == 0 ? t : k < c ? find(t->ch[0], k) : find(t->ch[1], k - c);
}

int cnt(node *t, T v) {
	if (!t) return 0;
	if (!t->ch[1]) return t->val < v;
	if (v > t->ch[1]->all) return count(t->ch[0]) + cnt(t->ch[1], v);
	if (v == t->ch[1]->all) return count(t->ch[0]);
	return cnt(t->ch[0], v);
}

void print(node *t) {
	if (!t) return;
	print(t->ch[0]);
	if (!t->ch[0]) printf(" %d", t->val);
	print(t->ch[1]);
}
