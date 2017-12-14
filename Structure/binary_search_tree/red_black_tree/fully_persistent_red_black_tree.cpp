
// merge/split (unable to solve range query)
using T = char;
T id = '*';
enum COL { BLACK, RED };

struct node;
node *update(node *t);

struct node {
	T val;
	node *ch[2];
	COL color;
	int level;
	int size;
	node() {}
	void init(T v) {
		val = v;
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

const int pmax = 5e8 / sizeof(node) * 2;	// the number of pooled nodes

int it = 0;
node pool[pmax];

int ranks(node *t) { return !t ? 0 : t->level; }

int count(node *t) { return !t ? 0 : t->size; }

node *update(node *t) {
	t->level = ranks(t->ch[0]) + (t->ch[0]->color == BLACK);
	t->size = count(t->ch[0]) + count(t->ch[1]);
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

node *submerge(node *l, node *r) {
	if (l->level < r->level) {
		node *c = submerge(l, r->ch[0]);
		if (r->color == BLACK && c->color == RED && c->ch[0]->color == RED) {
			if (r->ch[1]->color == BLACK) return new_node(c->ch[0], new_node(c->ch[1], r->ch[1], RED), BLACK);
			else return new_node(new_node(c->ch[0], c->ch[1], BLACK), new_node(r->ch[1]->ch[0], r->ch[1]->ch[1], BLACK), RED);
		}
		else return new_node(c, r->ch[1], r->color);
	}
	else if (l->level > r->level) {
		node *c = submerge(l->ch[1], r);
		if (l->color == BLACK && c->color == RED && c->ch[1]->color == RED) {
			if (l->ch[0]->color == BLACK) return new_node(new_node(l->ch[0], c->ch[0], RED), c->ch[1], BLACK);
			else return new_node(new_node(l->ch[0]->ch[0], l->ch[0]->ch[1], BLACK), new_node(c->ch[0], c->ch[1], BLACK), RED);
		}
		else return new_node(l->ch[0], c, l->color);
	}
	else {
		return new_node(l, r, RED);
	}
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	node *c = submerge(l, r);
	if (c->color == RED) {
		return new_node(c->ch[0], c->ch[1], BLACK);
	}
	return c;
}

pair<node*, node*> split(node *t, int k) {
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

node *insert(node *t, int k, int v) {
	pair<node*, node*> s = split(t, k);
	node *r = merge(s.first, new_leaf(v));
	r = merge(r, s.second);
	return r;
}

node *erase(node *t, int k) {
	pair<node*, node*> s1 = split(t, k), s2 = split(s1.second, 1);
	return merge(s1.first, s2.second);
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->ch[0]);
	return k < c ? find(t->ch[0], k) : k == c ? t : find(t->ch[1], k - (c + 1));
}

int cnt(node *t, int v) {
	if (!t) return 0;
	if (t->val < v) return count(t->ch[0]) + 1 + cnt(t->ch[1], v);
	if (t->val == v) return count(t->ch[0]);
	return cnt(t->ch[0], v);
}

node *copy(node *t, int a, int b, int c) {
	auto t1 = split(t, a);
	auto t2 = split(t1.second, b - a);
	auto t3 = split(t, c);
	return merge(t3.first, merge(t2.first, t3.second));
}

char pt[2000001];
int pit;

void getS(node* t) {
	if (!t) return;
	getS(t->ch[0]);
	if (t->val != id) pt[pit++] = t->val;
	getS(t->ch[1]);
}
void print(node* t) {
	pit = 0;
	getS(t);
	for (int i = 0; i < pit; i++) {
		printf("%c", pt[i]);
	}
	printf("\n");
}

node* build(int l, int r) {
	if (l + 1 == r) {
		return new_leaf(pt[l]);
	}
	return merge(build(l, (l + r) / 2), build((l + r) / 2, r));
}

int SIZE;

node* rebuild(node* t) {
	pit = 0;
	getS(t);
	it = 0;
	return build(0, min(pit, SIZE));
}
