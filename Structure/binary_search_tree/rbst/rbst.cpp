
// merge/split (unable to solve range query)
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = int;
struct node {
	T val;
	node *lch, *rch;
	int size;
	node(T v, node* l = nullptr, node* r = nullptr) : val(v), lch(l), rch(r), size(1) {}
};

int count(node *t) { return !t ? 0 : t->size; }
node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	return t;
}
node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	if (xor128() % (l->size + r->size) < (unsigned)l->size) {
		l->rch = merge(l->rch, r);
		return update(l);
	}
	else {
		r->lch = merge(l, r->lch);
		return update(r);
	}
}
pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
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
	if (t == nullptr) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

int cnt(node *t, T v) {
	if (t == nullptr) return 0;
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}

void change(node *t, int k, T v) {
	if (t == nullptr) return;
	int c = count(t->lch);
	if (k < c) {
		change(t->lch, k, v);
	}
	else if (k > c) {
		change(t->rch, k - (c + 1), v);
	}
	else {
		t->val = v;
	}
}

node *shift(node *t, int l, int r) {
	pair<node*, node*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}

void print(node *t) {
	if (t == nullptr) return;
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}

// merge/split (able to solve range query)
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

using T = int;
const T id = INT_MAX;
T op(T l, T r) {
	return min(l, r);
}

struct node {
	T val, all;
	node *lch, *rch;
	int size;
	node(T v, node* l = nullptr, node* r = nullptr) : val(v), all(v), lch(l), rch(r), size(1) {}
};

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id : t->all; }

node *update(node *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = op(que(t->lch), op(t->val, que(t->rch)));
	return t;
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	if (xor128() % (l->size + r->size) < (unsigned)l->size) {
		l->rch = merge(l->rch, r);
		return update(l);
	}
	else {
		r->lch = merge(l, r->lch);
		return update(r);
	}
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
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
	node *r = merge(s.first, new node(v));
	r = merge(r, s.second);
	return r;
}

node *erase(node *t, int k) {
	pair<node*, node*> s1 = split(t, k), s2 = split(s1.second, 1);
	if (s2.first) delete s2.first;
	return merge(s1.first, s2.second);
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

T find(node *t, int l, int r) {
	if (!t) return id;
	if (r < 0 || l >= count(t)) return id;
	if (l <= 0 && r >= count(t)) return t->all;
	int c = count(t->lch);
	return op(!t->lch ? id : find(t->lch, l, r), op(l <= c && c < r ? t->val : id, !t->rch ? id : find(t->rch, l - (c + 1), r - (c + 1))));
}

int cnt(node *t, T v) {
	if (!t) return 0;
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
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

node *shift(node *t, int l, int r) {
	pair<node*, node*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}

void print(node *t) {
	if (!t) return;
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}
