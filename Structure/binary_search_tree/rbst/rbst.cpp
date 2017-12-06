
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

node<T> *insert(node<T> *t, int k, T v) {
	pair<node<T>*, node<T>*> s = split(t, k);
	node<T> *r = merge(s.first, new node<T>(v));
	r = merge(r, s.second);
	return r;
}

node<T> *erase(node<T> *t, int k) {
	pair<node<T>*, node<T>*> s1 = split(t, k), s2 = split(s1.second, 1);
	if (s2.first != nullptr) delete s2.first;
	return merge(s1.first, s2.second);
}

node<T> *find(node<T> *t, int k) {
	if (t == nullptr) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}

int cnt(node<T> *t, T v) {
	if (t == nullptr) return 0;
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}

void change(node<T> *t, int k, T v) {
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

node<T> *shift(node<T> *t, int l, int r) {
	pair<node<T>*, node<T>*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}

void print(node<T> *t) {
	if (t == nullptr) return;
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}

// merge/split (able to solve range query)
int id = INT_MAX;
int merge(int l, int r) {
	return min(l, r);
}

unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

template <typename T>
struct node {
	T val, all;
	node<T> *lch, *rch;
	int size;
	node(T v, node<T>* l = nullptr, node<T>* r = nullptr) : val(v), all(v), lch(l), rch(r), size(1) {}
};

template <typename T>
int count(node<T> *t) { return t == nullptr ? 0 : t->size; }
template <typename T>
T que(node<T> *t) { return t == nullptr ? id : t->all; }
template <typename T>
node<T> *update(node<T> *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = merge(que(t->lch), merge(t->val, que(t->rch)));
	return t;
}
template <typename T>
node<T> *merge(node<T> *l, node<T> *r) {
	if (l == nullptr || r == nullptr) return l == nullptr ? r : l;
	if (xor128() % (l->size + r->size) < (unsigned)l->size) {
		l->rch = merge(l->rch, r);
		return update(l);
	}
	else {
		r->lch = merge(l, r->lch);
		return update(r);
	}
}
template <typename T>
pair<node<T>*, node<T>*> split(node<T> *t, int k) {
	if (t == nullptr) return make_pair(nullptr, nullptr);
	if (k <= count(t->lch)) {
		pair<node<T>*, node<T>*> s = split(t->lch, k);
		t->lch = s.second;
		return make_pair(s.first, update(t));
	}
	else {
		pair<node<T>*, node<T>*> s = split(t->rch, k - count(t->lch) - 1);
		t->rch = s.first;
		return make_pair(update(t), s.second);
	}
}
//--------------------------------------options---------------------------------------------------------
template <typename T>
node<T> *insert(node<T> *t, int k, T v) {
	pair<node<T>*, node<T>*> s = split(t, k);
	node<T> *r = merge(s.first, new node<T>(v));
	r = merge(r, s.second);
	return r;
}
template <typename T>
node<T> *erase(node<T> *t, int k) {
	pair<node<T>*, node<T>*> s1 = split(t, k), s2 = split(s1.second, 1);
	if (s2.first != nullptr) delete s2.first;
	return merge(s1.first, s2.second);
}
template <typename T>
node<T> *find(node<T> *t, int k) {
	if (t == nullptr) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}
template <typename T>
T find(node<T> *t, int l, int r) {
	if (t == nullptr) return id;
	if (r < 0 || l >= count(t)) return id;
	if (l <= 0 && r >= count(t)) return t->all;
	int c = count(t->lch);
	return merge(t->lch == nullptr ? id : find(t->lch, l, r), merge(l <= c && c < r ? t->val : id, t->rch == nullptr ? id : find(t->rch, l - (c + 1), r - (c + 1))));
}
template <typename T>
int cnt(node<T> *t, T v) {
	if (t == nullptr) return 0;
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}
template <typename T>
void change(node<T> *t, int k, T v) {
	if (t == nullptr) return;
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
template <typename T>
node<T> *shift(node<T> *t, int l, int r) {
	pair<node<T>*, node<T>*> s1 = split(t, l), s2 = split(s1.second, r - l), s3 = split(s2.second, 1);
	return merge(s1.first, merge(s3.first, merge(s2.first, s3.second)));
}
template <typename T>
void print(node<T> *t) {
	if (t == nullptr) return;
	print(t->lch);
	printf(" %d", t->val);
	print(t->rch);
}
