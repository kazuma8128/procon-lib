
template<typename U = unsigned, int B = 32>
class binary_trie {
	struct node {
		int cnt;
		node *ch[2];
		node() : cnt(0), ch{ nullptr, nullptr } {}
	};
	node* add(node* t, U val, int b = B - 1) {
		if (!t) t = new node;
		t->cnt += 1;
		if (b < 0) return t;
		bool f = (val >> (U)b) & (U)1;
		t->ch[f] = add(t->ch[f], val, b - 1);
		return t;
	}
	node* del(node* t, U val, int b = B - 1) {
		assert(t);
		t->cnt -= 1;
		if (t->cnt == 0) return nullptr;
		if (b < 0) return t;
		bool f = (val >> (U)b) & (U)1;
		t->ch[f] = del(t->ch[f], val, b - 1);
		return t;
	}
	U get(node* t, int k, U bias, int b = B - 1) const {
		if (b < 0) return 0;
		int f = (bias >> (U)b) & (U)1, m = t->ch[f] ? t->ch[f]->cnt : 0;
		if (k < m) return get(t->ch[f], k, bias, b - 1) | ((U)f << (U)b);
		return get(t->ch[!f], k - m, bias, b - 1) | ((U)(!f) << (U)b);
	}
	node *root;
public:
	binary_trie() : root(nullptr) {}
	int size() const {
		return root ? root->cnt : 0;
	}
	bool empty() const {
		return !root;
	}
	void insert(U val) {
		root = add(root, val);
	}
	void erase(U val) {
		root = del(root, val);
	}
	U max_element(U bias = 0) const {
		assert(root);
		return get(root, 0, ~bias);
	}
	U min_element(U bias = 0) const {
		assert(root);
		return get(root, 0, bias);
	}
	U kth_min(int k, U bias = 0) const {
		assert(0 <= k && k < size());
		return get(root, k, bias);
	}
	int lower_count(U val, U bias = 0) const { // count x : (x ^ bias) < val
		node *t = root;
		int res = 0;
		for (int b = B - 1; b >= 0 && t; b--) {
			bool f = (val >> (U)b) & (U)1;
			bool g = (bias >> (U)b) & (U)1;
			res += (f && t->ch[g]) ? t->ch[g]->cnt : 0;
			t = t->ch[f ^ g];
		}
		return res;
	}
	U lower_bound(U val, U bias = 0) const {
		return kth_min(lower_count(val, bias), bias);
	}
	int count(U val) const {
		if (!root) return 0;
		node *t = root;
		for (int i = B - 1; i >= 0; i--) {
			t = t->ch[(val >> (U)i) & (U)1];
			if (!t) return 0;
		}
		return t->cnt;
	}
};
