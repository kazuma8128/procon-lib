
// unsigned int only
struct node {
	node *l, *r;
	int cnt;
	node(node *l = nullptr, node *r = nullptr) : l(l), r(r), cnt(0) {}
};

class uint_PersistentTrie {
	vector<node*>root;
	int count(node *t) {
		return t == nullptr ? 0 : t->cnt;
	}
	unsigned get(node *t, int k, unsigned tmp, int b) {
		if (b < 0) return tmp;
		assert(t != nullptr);
		int c = count(t->l);
		return k < c ? get(t->l, k, tmp, b - 1) : get(t->r, k - c, tmp | (1 << b), b - 1);
	}
	node *add(node *t, unsigned val, int b) {
		if (t == nullptr) t = new node();
		else t = new node(*t);
		t->cnt++;
		if (b >= 0) {
			if (val & (1 << b)) {
				t->r = add(t->r, val, b - 1);
			}
			else {
				t->l = add(t->l, val, b - 1);
			}
		}
		return t;
	}
	node *sub(node *t, unsigned val, int b) {
		assert(t != nullptr);
		if (t->cnt == 1) {
			return nullptr;
		}
		t = new node(*t);
		t->cnt--;
		if (b >= 0) {
			if (val & (1 << b)) {
				t->r = sub(t->r, val, b - 1);
			}
			else {
				t->l = sub(t->l, val, b - 1);
			}
		}
		return t;
	}
public:
	uint_PersistentTrie() : root(1, nullptr) {}
	int getversion() {
		return root.size() - 1;
	}
	int size(int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		return count(root[ver]);
	}
	void insert(unsigned val, int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		root.push_back(add(root[ver], val, 31));
	}
	void erase(unsigned val, int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		root.push_back(sub(root[ver], val, 31));
	}
	unsigned max(unsigned bias, int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		assert(count(root[ver]) >= 1);
		node *t = root[ver];
		unsigned res = 0;
		for (int i = 31; i >= 0; i--) {
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
	unsigned max(int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		assert(count(root[ver]) >= 1);
		return get(root[ver], count(root[ver]) - 1, 0, 31);
	}
	unsigned min(unsigned bias, int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		assert(count(root[ver]) >= 1);
		node *t = root[ver];
		unsigned res = 0;
		for (int i = 31; i >= 0; i--) {
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
	unsigned min(int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		assert(count(root[ver]) >= 1);
		return get(root[ver], 0, 0, 31);
	}
	unsigned get(int k, int ver = -1) {
		if (ver == -1) ver = root.size() - 1;
		assert(0 <= ver && ver < (int)root.size());
		assert(0 <= k && k < count(root[ver]));
		return get(root[ver], k, 0, 31);
	}
};
