
// insert/erase
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = int;
const T id1 = INT_MAX;
const T id2 = -1;
T op1(T l, T r) {
	return min(l, r);
}
T op2(T l, T r) {
	return r == id2 ? l : r;
}

struct node {
	T val, all, lazy;
	node *ch[2];
	unsigned int pri;
	int size;
	node(T v, unsigned int p, node* l = nullptr, node* r = nullptr)
		: val(v), all(v), lazy(id2), pri(p), size(1) {
		ch[0] = l; ch[1] = r;
	}
};

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id1 : op2(t->all, t->lazy); }

node *update(node *t) {
	t->size = count(t->ch[0]) + count(t->ch[1]) + 1;
	t->all = op1(que(t->ch[0]), op1(t->val, que(t->ch[1])));
	return t;
}

node *push(node *t) {
	if (!t || t->lazy == id2) return t;
	if (t->ch[0]) t->ch[0]->lazy = op2(t->ch[0]->lazy, t->lazy);
	if (t->ch[1]) t->ch[1]->lazy = op2(t->ch[1]->lazy, t->lazy);
	t->val = op2(t->val, t->lazy);
	t->all = op1(que(t->ch[0]), op1(t->val, que(t->ch[1])));
	t->lazy = id2;
	return t;
}

node *rotate(node *t, int b) {
	node *s = t->ch[1 - b];
	t = push(t);
	s = push(s);
	t->ch[1 - b] = s->ch[b];
	s->ch[b] = t;
	update(t); update(s);
	return s;
}

node *insert(node *t, int k, T v, unsigned int pri) {
	if (!t) return new node(v, pri);
	t = push(t);
	int c = count(t->ch[0]), b = (k > c);
	t->ch[b] = insert(t->ch[b], k - (b ? (c + 1) : 0), v, pri);
	update(t);
	if (t->pri > t->ch[b]->pri) t = rotate(t, 1 - b);
	return t;
}

node *erase(node *t) {
	if (!t) return nullptr;
	if (!t->ch[0] && !t->ch[1]) {
		delete t;
		return nullptr;
	}
	t = push(t);
	int b = (t->ch[0] && t->ch[1])
			? (t->ch[0]->pri > t->ch[1]->pri)
			: (t->ch[0] != nullptr);
	t = rotate(t, b);
	t->ch[b] = erase(t->ch[b]);
	update(t);
	return t;
}

node *erase(node *t, int k) {
	if (!t) return nullptr;
	t = push(t);
	int c = count(t->ch[0]);
	if (k < c) {
		t->ch[0] = erase(t->ch[0], k);
		update(t);
	}
	else if (k > c) {
		t->ch[1] = erase(t->ch[1], k - (c + 1));
		update(t);
	}
	else {
		t = erase(t);
	}
	return t;
}
//--------------------------------------options---------------------------------------------------------

node *merge(node *l, node *r) {
	node *p = new node(0, 0);
	p->ch[0] = l, p->ch[1] = r;
	p = erase(p);
	return p;
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
	node *p = insert(t, k, 0, 0);
	auto res = make_pair(p->ch[0], p->ch[1]);
	delete p;
	return res;
}

T find(node *t, int l, int r) {
	if (!t) return id1;
	if (r < 0 || l >= count(t)) return id1;
	t = push(t);
	if (l <= 0 && r >= t->size) return t->all;
	int c = count(t->ch[0]);
	return op1(!t->ch[0] ? id1 : find(t->ch[0], l, r),
		op1(l <= c && c < r ? t->val : id1,
			!t->ch[1] ? id1 : find(t->ch[1], l - (c + 1), r - (c + 1)))
	);
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->ch[0]);
	return k < c ? find(t->ch[0], k) : k == c ? t : find(t->ch[1], k - (c + 1));
}

int cnt(node *t, T v) {
	if (!t) return 0;
	t = push(t);
	if (t->val < v) return count(t->ch[0]) + 1 + cnt(t->ch[1], v);
	if (t->val == v) return count(t->ch[0]);
	return cnt(t->ch[0], v);
}

node *update(node *t, int l, int r, T v) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	t2.first->lazy = op2(t2.first->lazy, v);
	return merge(t1.first, merge(t2.first, t2.second));
}

void print(node *t) {
	if (!t) return;
	print(t->ch[0]);
	printf(" %d", t->val);
	print(t->ch[1]);
}

node *shift(node *t, int l, int r) {
	pair<node*, node*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}

void change(node *t, int k, T v) {
	if (!t) return;
	int c = count(t->ch[0]);
	if (k < c) {
		change(t->ch[0], k, v);
		update(t);
	}
	else if (k > c) {
		change(t->ch[1], k - (c + 1), v);
		update(t);
	}
	else {
		t->val = v;
		update(t);
	}
}

// merge/split
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = int;
const T id1 = INT_MAX;
const T id2 = -1;
T op1(T l, T r) {
	return min(l, r);
}
T op2(T l, T r) {
	return r == id2 ? l : r;
}

struct node {
	T val, all, lazy;
	node *lch, *rch;
	unsigned int pri;
	int size;
	node(T v, unsigned int p, node *l = nullptr, node *r = nullptr)
		: val(v), all(v), lazy(id2), lch(l), rch(r), pri(p), size(1) {}
};

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id1 : op2(t->all, t->lazy); }

node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = op1(que(t->lch), op1(t->val, que(t->rch)));
	return t;
}

node *push(node *t) {
	if (!t || t->lazy == id2) return t;
	if (t->lch) t->lch->lazy = op2(t->lch->lazy, t->lazy);
	if (t->rch) t->rch->lazy = op2(t->rch->lazy, t->lazy);
	t->val = op2(t->val, t->lazy);
	t->all = op1(que(t->lch), op1(t->val, que(t->rch)));
	t->lazy = id2;
	return t;
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	if (l->pri > r->pri) {
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
//--------------------------------------options---------------------------------------------------------

node *insert(node *t, int k, T v) {
	pair<node*, node*> s = split(t, k);
	node *r = merge(s.first, new node(v, xor128()));
	r = merge(r, s.second);
	return r;
}

node *erase(node *t, int k) {
	pair<node*, node*> s1 = split(t, k), s2 = split(s1.second, 1);
	if (s2.first != nullptr) delete s2.first;
	return merge(s1.first, s2.second);
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

T find(node *t, int l, int r) {
	if (!t) return id1;
	if (r < 0 || l >= count(t)) return id1;
	t = push(t);
	if (l <= 0 && r >= count(t)) return t->all;
	int c = count(t->lch);
	return op1(!t->lch ? id1 : find(t->lch, l, r),
		op1(l <= c && c < r ? t->val : id1,
			!t->rch ? id1 : find(t->rch, l - (c + 1), r - (c + 1)))
	);
}

int cnt(node *t, T v) {
	if (!t) return 0;
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

void print(node *t) {
	if (!t) return;
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}

node *shift(node *t, int l, int r) {
	pair<node*, node*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}

void change(node *t, int k, T v) {
	if (!t) return;
	int c = count(t->lch);
	if (k < c) {
		change(t->lch, k, v);
		update(t);
	}
	else if (k > c) {
		change(t->rch, k - (c + 1), v);
		update(t);
	}
	else {
		t->val = v;
		update(t);
	}
}
