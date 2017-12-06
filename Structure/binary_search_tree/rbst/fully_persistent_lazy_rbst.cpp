
// merge/split
using ll = long long;
ll id = 0;
ll id2 = 0;
ll merge(ll l, ll r) {
	return l + r;
}
ll merge2(ll l, ll r) {
	return l + r;
}

unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

template <typename T>
struct node {
	T val, all, lazy;
	node<T> *lch, *rch;
	int size;
	void Init(T v, node<T>* l = nullptr, node<T>* r = nullptr) {
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
node<ll> pool[pmax];

template <typename T>
int count(node<T> *t) { return t == nullptr ? 0 : t->size; }
template <typename T>
T que(node<T> *t) { return t == nullptr ? id : t->all + t->lazy * t->size; }
template <typename T>
node<T> *update(node<T> *t) {
	t->size = count(t->lch) + count(t->rch) + 1;
	t->all = merge(que(t->lch), merge(t->val, que(t->rch)));
	return t;
}
template <typename T>
node<T> *fix(node<T> *t) {
	if (t == nullptr) return t;
	pool[it] = *t;
	return &pool[it++];
}
template <typename T>
node<T> *push(node<T> *t) {
	if (t == nullptr) return t;
	t = fix(t);
	if (t->lch != nullptr) {
		t->lch = fix(t->lch);
		t->lch->lazy = merge2(t->lch->lazy, t->lazy);
	}
	if (t->rch != nullptr) {
		t->rch = fix(t->rch);
		t->rch->lazy = merge2(t->rch->lazy, t->lazy);
	}
	t->val = merge2(t->val, t->lazy);
	t->all = merge(que(t->lch), merge(t->val, que(t->rch)));
	t->lazy = id2;
	return t;
}
template <typename T>
node<T> *merge(node<T> *l, node<T> *r) {
	if (l == nullptr || r == nullptr) return l == nullptr ? r : l;
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
template <typename T>
pair<node<T>*, node<T>*> split(node<T> *t, int k) {
	if (t == nullptr) return make_pair(nullptr, nullptr);
	t = push(t);
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
template <typename T>
node<T> *find(node<T> *t, int k) {
	if (t == nullptr) return t;
	int c = count(t->lch);
	return k < c ? find(t->lch, k) : k == c ? t : find(t->rch, k - (c + 1));
}
template <typename T>
node<T>* find(node<T> *t, int l, int r, T& res) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	res = que(t2.first);
	return t;
}
template <typename T>
void print(node<T> *t) {
	if (t == nullptr) return;
	t = push(t);
	print(t->lch);
	printf(" %lld", t->val);
	print(t->rch);
}
template <typename T>
int cnt(node<T> *t, T v) {
	if (t == nullptr) return 0;
	t = push(t);
	if (t->val < v) return count(t->lch) + 1 + cnt(t->rch, v);
	if (t->val == v) return count(t->lch);
	return cnt(t->lch, v);
}
template <typename T>
node<T> *update(node<T> *t, int l, int r, T v) {
	auto t1 = split(t, l);
	auto t2 = split(t1.second, r - l);
	t2.first->lazy = merge2(t2.first->lazy, v);
	return merge(t1.first, merge(t2.first, t2.second));
}
template<typename T>
node<T> *copy(node<T> *t, int a, int b, int c, int d) {
	auto t1 = split(t, a);
	auto t2 = split(t1.second, b - a);
	auto t3 = split(t, c);
	auto t4 = split(t3.second, d - c);
	return merge(t1.first, merge(t4.first, t2.second));
}

int xit;
ll x[200000];
int N;

node<ll>* build() {
	it = 0;
	node<ll> *res = nullptr;
	for (int i = 0; i < N; i++) {
		pool[it].Init(x[i]);
		res = merge(res, &pool[it++]);
	}
	return res;
}

void up(node<ll> *t) {
	if (t == nullptr) return;
	t = push(t);
	up(t->lch);
	if (xit >= N) return;
	x[xit++] = t->val;
	up(t->rch);
}

node<ll>* rebuild(node<ll>* t) {
	xit = 0;
	up(t);
	return build();
}
