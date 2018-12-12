
template<typename U = unsigned, int B = 32>
class binary_trie {
	struct node {
		U pos;
		int cnt, sum;
		node *ch[2];
		node(U p) : pos(p), cnt(0), sum(0), ch{ nullptr, nullptr } {}
	};
	node* add(node* t, U val, int c, int b = B - 1) {
		if (!t) t = new node(val);
		t->sum += c;
		if (val == t->pos) {
			t->cnt += c;
			return t;
		}
		bool f = (val >> (U)b) & (U)1;
		bool g = (t->pos >> (U)b) & (U)1;
		if (f == g && (f ^ (val > t->pos))) {
			swap(val, t->pos);
			swap(c, t->cnt);
		}
		t->ch[f] = add(t->ch[f], val, c, b - 1);
		return t;
	}
	U get_min(node* t, U bias, int b = B - 1) const {
		if (t->cnt == t->sum) return t->pos ^ bias;
		bool f = (bias >> (U)b) & (U)1;
		if (!t->ch[f] || !t->ch[f]->sum) f = !f;
		int res = get_min(t->ch[f], bias, b - 1);
		if (t->cnt) res = min(res, t->pos ^ bias);
		return res;
	}
	node *root;
public:
	binary_trie() : root(nullptr) {}
	int size() const {
		return root ? root->sum : 0;
	}
	bool empty() const {
		return !root;
	}
	void insert(U val) {
		root = add(root, val, 1);
	}
	void erase(U val) {
		root = add(root, val, -1);
	}
	U min_element(U bias = 0) const {
		assert(root);
		return get_min(root, bias) ^ bias;
	}
	U max_element(U bias = 0) const {
		assert(root);
		return get_min(root, ~bias) ^ bias;
	}
	U kth_min(int k) {
		assert(0 <= k && k < size());
		node *t = root;
		while (true) {
			int c = t->ch[0] ? t->ch[0]->sum : 0;
			if (c <= k && k < c + t->cnt) return t->pos;
			if (k < c) t = t->ch[0];
			else k -= c + t->cnt, t = t->ch[1];
		}
	}
	int lower_count(U val, U bias = 0) const { // count x : (x ^ bias) < val
		node *t = root;
		int res = 0;
		for (int b = B - 1; b >= 0 && t; b--) {
			bool f = (val >> (U)b) & (U)1;
			bool g = (bias >> (U)b) & (U)1;
			res += (f && t->ch[g]) ? t->ch[g]->sum : 0;
			res += (t->pos ^ bias) < val ? t->cnt : 0;
			t = t->ch[f ^ g];
		}
		return res;
	}
	int count(U val) const {
		node *t = root;
		int b = B - 1;
		while (t) {
			if (t->pos == val) return t->cnt;
			t = t->ch[(val >> (U)b--) & (U)1];
		}
		return 0;
	}
};
