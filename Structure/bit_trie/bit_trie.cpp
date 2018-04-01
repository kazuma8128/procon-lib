
// bit trie
struct node {
	node *l, *r;
	int cnt;
	node(node *l = nullptr, node *r = nullptr) : l(l), r(r), cnt(0) {}
};

template<typename U = unsigned, const int B = 32>
class bit_trie {
	node *root;
	int count(node *t) {
		return t == nullptr ? 0 : t->cnt;
	}
	U get(node *t, int k, U tmp, int b) {
		if (b < 0) return tmp;
		assert(t != nullptr);
		int c = count(t->l);
		return k < c ? get(t->l, k, tmp, b - 1) : get(t->r, k - c, tmp | ((U)1 << (U)b), b - 1);
	}
	node *add(node *t, U val, int b) {
		if (t == nullptr) t = new node();
		t->cnt++;
		if (b >= 0) {
			if (val & ((U)1 << b)) {
				t->r = add(t->r, val, b - 1);
			}
			else {
				t->l = add(t->l, val, b - 1);
			}
		}
		return t;
	}
	node *sub(node *t, U val, int b) {
		assert(t != nullptr);
		t->cnt--;
		if (b >= 0) {
			if (val & ((U)1 << (U)b)) {
				t->r = sub(t->r, val, b - 1);
			}
			else {
				t->l = sub(t->l, val, b - 1);
			}
		}
		if (t->cnt == 0) {
			delete t;
			return nullptr;
		}
		return t;
	}
public:
	bit_trie() : root(nullptr) {}
	int size() {
		return count(root);
	}
	void insert(U val) {
		root = add(root, val, B - 1);
	}
	void erase(U val) {
		root = sub(root, val, B - 1);
	}
	bool find(U val) {
		node *t = root;
		for (int i = B - 1; i >= 0; i--) {
			if (val & ((U)1 << (U)i)) {
				if (t->r == nullptr) return false;
				t = t->r;
			}
			else {
				if (t->l == nullptr) return false;
				t = t->l;
			}
		}
		return true;
	}
	U max(U bias = 0) {
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			if (bias & ((U)1 << (U)i)) {
				if (t->l != nullptr) {
					t = t->l;
				}
				else {
					t = t->r;
					res |= (U)1 << (U)i;
				}
			}
			else {
				if (t->r != nullptr) {
					t = t->r;
					res |= (U)1 << (U)i;
				}
				else {
					t = t->l;
				}
			}
		}
		return res;
	}
	U min(U bias = 0) {
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			if (bias & ((U)1 << (U)i)) {
				if (t->r != nullptr) {
					t = t->r;
					res |= (U)1 << (U)i;
				}
				else {
					t = t->l;
				}
			}
			else {
				if (t->l != nullptr) {
					t = t->l;
				}
				else {
					t = t->r;
					res |= (U)1 << (U)i;
				}
			}
		}
		return res;
	}
	U operator[](int k) {
		assert(0 <= k && k < count(root));
		return get(root, k, 0, B - 1);
	}
};

// non-recursive bit trie
struct node {
	node *p, *l, *r;
	int cnt;
	node(node *p, node *l = nullptr, node *r = nullptr) : p(p), l(l), r(r), cnt(0) {}
};

template<typename U = unsigned, const int B = 32>
class bit_trie {
	node *root;
public:
	bit_trie() : root(new node(nullptr)) {}
	int size() {
		return root->cnt;
	}
	void insert(U val) {
		// if (find(val)) return; // in case of multiset, erase this line
		node *t = root;
		for (int i = B - 1; i >= 0; i--) {
			t->cnt++;
			if (val & (1 << i)) {
				if (t->r == nullptr) {
					t->r = new node(t);
				}
				t = t->r;
			}
			else {
				if (t->l == nullptr) {
					t->l = new node(t);
				}
				t = t->l;
			}
		}
		t->cnt++;
	}
	void erase(U val) {
		node *t = root, *ne;
		for (int i = B - 1; i >= 0; i--) {
			t->cnt--;
			if (val & (1 << i)) {
				if (t->r == nullptr) return;
				t = t->r;
			}
			else {
				if (t->l == nullptr) return;
				t = t->l;
			}
		}
		t->cnt--;
		while (t != root && t->cnt == 0) {
			ne = t->p;
			(t == ne->l ? ne->l : ne->r) = nullptr;
			delete t;
			t = ne;
		}
	}
	bool find(U val) {
		node *t = root;
		for (int i = B - 1; i >= 0; i--) {
			if (val & (1 << i)) {
				if (t->r == nullptr) return false;
				t = t->r;
			}
			else {
				if (t->l == nullptr) return false;
				t = t->l;
			}
		}
		return true;
	}
	U max(U bias = 0) {
		assert(root->cnt >= 1);
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			if (bias & (1 << i)) {
				if (t->l != nullptr) {
					t = t->l;
				}
				else {
					t = t->r;
					res |= 1 << i;
				}
			}
			else {
				if (t->r != nullptr) {
					t = t->r;
					res |= 1 << i;
				}
				else {
					t = t->l;
				}
			}
		}
		return res;
	}
	U min(U bias = 0) {
		assert(root->cnt >= 1);
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			if (bias & (1 << i)) {
				if (t->r != nullptr) {
					t = t->r;
					res |= 1 << i;
				}
				else {
					t = t->l;
				}
			}
			else {
				if (t->l != nullptr) {
					t = t->l;
				}
				else {
					t = t->r;
					res |= 1 << i;
				}
			}
		}
		return res;
	}
};
