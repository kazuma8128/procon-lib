
const int PMAX = 2e6;

template<typename U = unsigned, const int B = 32>
class lazy_bit_trie {
	struct node {
		node *l, *r;
		int cnt;
		U lazy;
		node() {}
		void init(node *left = nullptr, node *right = nullptr) {
			l = left;
			r = right;
			cnt = lazy = 0;
		}
	};

	node *root;
	int it;
	vector<node> pool;
	node *new_node(node *l = nullptr, node *r = nullptr) {
		pool[it].init(l, r);
		return &pool[it++];
	}
	int count(node *t) {
		return t == nullptr ? 0 : t->cnt;
	}
	node *push(node *t, int b) {
		if (t == nullptr || t->lazy == 0) return t;
		if (t->lazy & ((U)1 << (U)b)) {
			swap(t->l, t->r);
			t->lazy ^= ((U)1 << (U)b);
		}
		if (t->l != nullptr) t->l->lazy ^= t->lazy;
		if (t->r != nullptr) t->r->lazy ^= t->lazy;
		t->lazy = 0;
		return t;
	}
	U get(node *t, int k, U tmp, int b) {
		if (b < 0) return tmp;
		assert(t != nullptr);
		t = push(t, b);
		int c = count(t->l);
		return k < c ? get(t->l, k, tmp, b - 1) : get(t->r, k - c, tmp | ((U)1 << (U)b), b - 1);
	}
	node *add(node *t, U val, int b) {
		if (t == nullptr) t = new_node();
		t = push(t, b);
		t->cnt++;
		if (b >= 0) {
			if (val & ((U)1 << (U)b)) {
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
		t = push(t, b);
		t->cnt--;
		if (b >= 0) {
			if (val & ((U)1 << (U)b)) {
				t->r = sub(t->r, val, b - 1);
			}
			else {
				t->l = sub(t->l, val, b - 1);
			}
		}
		if (t->cnt == 0) return nullptr;
		return t;
	}
	int lb(node *t, U val, U tmp, int b) {
		if (b < 0) return count(t);
		if (t == nullptr) return 0;
		t = push(t, b);
		if (tmp + ((U)1 << (U)b) < val) return count(t->l) + lb(t->r, val, tmp | ((U)1 << (U)b), b - 1);
		return lb(t->l, val, tmp, b - 1);
	}
	int ub(node *t, U val, U tmp, int b) {
		if (b < 0) return count(t);
		if (t == nullptr) return 0;
		t = push(t, b);
		if (tmp + ((U)1 << (U)b) <= val) return count(t->l) + ub(t->r, val, tmp | ((U)1 << (U)b), b - 1);
		return ub(t->l, val, tmp, b - 1);
	}
public:
	lazy_bit_trie() : root(nullptr), it(0), pool(PMAX) {}
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
			t = push(t, i);
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
	U max() {
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			t = push(t, i);
			if (t->r != nullptr) {
				t = t->r;
				res |= (U)1 << (U)i;
			}
			else {
				t = t->l;
			}
		}
		return res;
	}
	U min() {
		node *t = root;
		U res = 0;
		for (int i = B - 1; i >= 0; i--) {
			t = push(t, i);
			if (t->l != nullptr) {
				t = t->l;
			}
			else {
				t = t->r;
				res |= (U)1 << (U)i;
			}
		}
		return res;
	}
	int lower_bound(U val) {
		return lb(root, val, 0, B - 1);
	}
	int upper_bound(U val) {
		return ub(root, val, 0, B - 1);
	}
	U operator[](int k) {
		assert(0 <= k && k < count(root));
		return get(root, k, 0, B - 1);
	}
	void XOR(U val) {	// xor all elements by val
		if (root == nullptr) root = new_node();
		root->lazy ^= val;
	}
};
