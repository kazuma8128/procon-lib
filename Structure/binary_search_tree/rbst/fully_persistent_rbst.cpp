
// merge/split (unable to solve range query)
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = char;
struct node {
	T val;
	node *lch, *rch;
	int size;
	void init(T v) {
		val = v;
		lch = rch = nullptr;
		size = 1;
	}
};

const int pmax = 500000000 / sizeof(node) * 2;

int it = 0;
node pool[pmax];

int count(node *t) { return !t ? 0 : t->size; }

node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	return t;
}

node *fix(node *t) {
	pool[it] = *t;
	return &pool[it++];
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	if (xor128() % (l->size + r->size) < (unsigned)l->size) {
		l = fix(l);
		l->rch = merge(l->rch, r);
		return update(l);
	}
	else {
		r = fix(r);
		r->lch = merge(l, r->lch);
		return update(r);
	}
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
	if (k <= count(t->lch)) {
		pair<node*, node*> s = split(t->lch, k);
		t = fix(t);
		t->lch = s.second;
		return make_pair(s.first, update(t));
	}
	else {
		pair<node*, node*> s = split(t->rch, k - count(t->lch) - 1);
		t = fix(t);
		t->rch = s.first;
		return make_pair(update(t), s.second);
	}
}

node *insert(node *t, int k, T v) {
	pair<node*, node*> s = split(t, k);
	pool[it++].init(v);
	node *r = merge(s.first, &pool[it - 1]);
	r = merge(r, s.second);
	return r;
}

node *erase(node *t, int k) {
	pair<node*, node*> s1 = split(t, k), s2 = split(s1.second, 1);
	return merge(s1.first, s2.second);
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

int cnt(node *t, T v) {
	if (!t) return 0;
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}

node *copy(node *t, int a, int b, int c) {
	auto t1 = split(t, a);
	auto t2 = split(t1.second, b - a);
	auto t3 = split(t, c);
	return merge(t3.first, merge(t2.first, t3.second));
}

//------------------------------------------options-----------------------------------------------------
char pt[1000001];
int pit;

void getS(node* t) {
	if (!t) return;
	getS(t->lch);
	pt[pit++] = t->val;
	getS(t->rch);
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
		pool[it].init(pt[l]);
		return &pool[it++];
	}
	return merge(build(l, (l + r) / 2), build((l + r) / 2, r));
}
node* rebuild(node* t) {
	pit = 0;
	getS(t);
	it = 0;
	return build(0, pit);
}
