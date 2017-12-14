
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
	int size;
	node(T v, node *l = nullptr, node *r = nullptr) : val(v), all(v), lazy(id2), size(1) {
		ch[0] = l; ch[1] = r;
	}
};

int count(node *t) { return !t ? 0 : t->size; }

T que(node *t) { return !t ? id1 : op2(t->all, t->lazy); } // changed for each query

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

node *splay(node *t, int k) {
	assert(t && k < count(t));
	t = push(t);
	int c = count(t->ch[0]);
	if (k < c) {
		int cc = count(t->ch[0]->ch[0]);
		if (k < cc) {
			t->ch[0] = push(t->ch[0]);
			t->ch[0]->ch[0] = splay(t->ch[0]->ch[0], k);
			t = rotate(t, 1);
			t = rotate(t, 1);
		}
		else if (k > cc) {
			t->ch[0] = push(t->ch[0]);
			t->ch[0]->ch[1] = splay(t->ch[0]->ch[1], k - (cc + 1));
			t->ch[0] = rotate(t->ch[0], 0);
			t = rotate(t, 1);
		}
		else {
			t = rotate(t, 1);
		}
	}
	else if (k > c) {
		k -= c + 1;
		int cc = count(t->ch[1]->ch[0]);
		if (k < cc) {
			t->ch[1] = push(t->ch[1]);
			t->ch[1]->ch[0] = splay(t->ch[1]->ch[0], k);
			t->ch[1] = rotate(t->ch[1], 1);
			t = rotate(t, 0);
		}
		else if (k > cc) {
			t->ch[1] = push(t->ch[1]);
			t->ch[1]->ch[1] = splay(t->ch[1]->ch[1], k - (cc + 1));
			t = rotate(t, 0);
			t = rotate(t, 0);
		}
		else {
			t = rotate(t, 0);
		}
	}
	t = push(t);
	return t;
}

node *merge(node *l, node *r) {
	if (!l || !r) return !l ? r : l;
	l = splay(l, count(l) - 1);
	l->ch[1] = r;
	update(l);
	return l;
}

pair<node*, node*> split(node *t, int k) {
	if (!t) return make_pair(nullptr, nullptr);
	if (k >= count(t)) return make_pair(t, nullptr);
	t = splay(t, k);
	auto s = t->ch[0];
	t->ch[0] = nullptr;
	return make_pair(s, update(t));
}

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

T find(node *t, int l, int r) {
	if (!t) return id1;
	if (r < 0 || l >= count(t)) return id1;
	t = push(t);
	if (l <= 0 && r >= t->size) return t->all;
	int c = count(t->ch[0]);
	return op1(!t->ch[0] ? id1 : find(t->ch[0], l, r), op1(l <= c && c < r ? t->val : id1, !t->ch[1] ? id1 : find(t->ch[1], l - (c + 1), r - (c + 1))));
}

node *find(node *t, int k) {
	if (!t) return t;
	int c = count(t->ch[0]);
	return k < c ? find(t->ch[0], k) : k == c ? t : find(t->ch[1], k - (c + 1));
}

int cnt(node *t, T v) {
	if (!t) return 0;
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
