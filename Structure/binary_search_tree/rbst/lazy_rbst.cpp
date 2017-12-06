
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
	int size;
	node(T v, node* l = nullptr, node* r = nullptr)
		: val(v), all(v), lazy(id2), lch(l), rch(r), size(1) {}
};

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id1 : op2(t->all, t->lazy); } // changed for each query

node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = op1(que(t->lch), op1(t->val, que(t->rch)));
	return t;
}

node *push(node *t) {
	if (!t || t->lazy == id2) return t;
	if (t->lch != nullptr) {
		t->lch->lazy = op2(t->lch->lazy, t->lazy);
	}
	if (t->rch != nullptr) {
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

node *insert(node *t, int k, T v) {
	pair<node*, node*> s = split(t, k);
	node *r = merge(s.first, new node(v));
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

int cnt(node *t, T v) {
	if (!t) return 0;
	t = push(t);
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
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

node *update(node *t, int l, int r, T v) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	t2.first->lazy = op2(t2.first->lazy, v);
	return merge(t1.first, merge(t2.first, t2.second));
}

void print(node *t) {
	if (!t) return;
	t = push(t);
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}
