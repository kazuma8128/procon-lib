
// merge/split
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = long long;
T id1 = 0;
T id2 = 0;
T op1(T l, T r) {
	return l + r;
}
T op2(T l, T r) {
	return l + r;
}

struct node {
	T val, all, lazy;
	node *lch, *rch;
	int size;
	void Init(T v, node* l = nullptr, node* r = nullptr) {
		val = v;
		all = v;
		lazy = id2;
		lch = l;
		rch = r;
		size = 1;
	}
};

const int pmax = 3e7;

int it;
node pool[pmax];

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id1 : t->all + t->lazy * t->size; }

node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = op1(que(t->lch), op1(t->val, que(t->rch)));
	return t;
}

node *fix(node *t) {
	if (!t) return t;
	assert(it < pmax);
	pool[it] = *t;
	return &pool[it++];
}

node *push(node *t) {
	if (!t) return t;
	t = fix(t);
	if (t->lch) {
		t->lch = fix(t->lch);
		t->lch->lazy = op2(t->lch->lazy, t->lazy);
	}
	if (t->rch) {
		t->rch = fix(t->rch);
		t->rch->lazy = op2(t->rch->lazy, t->lazy);
	}
	t->val = op2(t->val, t->lazy);
	t->all = op1(que(t->lch), op1(t->val, que(t->rch)));
	t->lazy = id2;
	return t;
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	if (xor128() % (l->size + r->size) < (unsigned)l->size) {
		l = push(l);
		l->rch = merge(l->rch, r);
		return update(l);
	}
	else {
		r = push(r);
		r->lch = merge(l, r->lch);
		return update(r);
	}
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
	t = push(t);
	if (k <= count(t->lch)) {
		pair<node*, node*> s = split(t->lch, k);
		t->lch = s.second;
		return make_pair(s.first, update(t));
	}
	else {
		pair<node*, node*> s = split(t->rch, k - count(t->lch) - 1);
		t->rch = s.first;
		return make_pair(update(t), s.second);
	}
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

node* find(node *t, int l, int r, T& res) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	res = que(t2.first);
	return t;
}

void print(node *t) {
	if (!t) return;
	t = push(t);
	print(t->lch);
	printf(" %lld", t->val);
	print(t->rch);
}

int cnt(node *t, T v) {
	if (!t) return 0;
	t = push(t);
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}

node *update(node *t, int l, int r, T v) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	t2.first->lazy = op2(t2.first->lazy, v);
	return merge(t1.first, merge(t2.first, t2.second));
}

node *copy(node *t, int a, int b, int c, int d) {
	auto t1 = split(t, a);
	auto t2 = split(t1.second, b - a);
	auto t3 = split(t, c);
	auto t4 = split(t3.second, d - c);
	return merge(t1.first, merge(t4.first, t2.second));
}
int xit;
T x[200000];
int N;

node* build() {
	it = 0;
	node *res = nullptr;
	for (int i = 0; i < N; i++) {
		pool[it].Init(x[i]);
		res = merge(res, &pool[it++]);
	}
	return res;
}

void up(node *t) {
	if (!t) return;
	t = push(t);
	up(t->lch);
	x[xit++] = t->val;
	up(t->rch);
}

node* rebuild(node* t) {
	xit = 0;
	up(t);
	return build();
}
