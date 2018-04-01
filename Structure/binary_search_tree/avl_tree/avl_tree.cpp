
// insert/erase (set type)
template <typename T>
class avl_tree {
	struct node {
		T val;
		node *ch[2];
		int dep, size;
		node(T v, node* l = nullptr, node* r = nullptr) : val(v), dep(1), size(1) {
			ch[0] = l; ch[1] = r;
		}
	};
	int depth(node *t) { return t == nullptr ? 0 : t->dep; }
	int count(node *t) { return t == nullptr ? 0 : t->size; }
	node *update(node *t) {
		t->dep = max(depth(t->ch[0]), depth(t->ch[1])) + 1;
		t->size = count(t->ch[0]) + count(t->ch[1]) + 1;
		return t;
	}
	node *rotate(node *t, int b) {
		node *s = t->ch[1 - b];
		t->ch[1 - b] = s->ch[b];
		s->ch[b] = t;
		t = update(t);
		s = update(s);
		return s;
	}
	node *fetch(node *t) {
		if (t == nullptr) return t;
		if (depth(t->ch[0]) - depth(t->ch[1]) == 2) {
			if (depth(t->ch[0]->ch[1]) > depth(t->ch[0]->ch[0])) {
				t->ch[0] = rotate(t->ch[0], 0);
			}
			t = rotate(t, 1);
		}
		else if (depth(t->ch[0]) - depth(t->ch[1]) == -2) {
			if (depth(t->ch[1]->ch[0]) > depth(t->ch[1]->ch[1])) {
				t->ch[1] = rotate(t->ch[1], 1);
			}
			t = rotate(t, 0);
		}
		return t;
	}
	node *insert(node *t, int k, T v) {
		if (t == nullptr) return new node(v);
		int c = count(t->ch[0]), b = (k > c);
		t->ch[b] = insert(t->ch[b], k - (b ? (c + 1) : 0), v);
		update(t);
		return fetch(t);
	}
	node *erase(node *t) {
		if (t == nullptr) return nullptr;
		if (t->ch[0] == nullptr && t->ch[1] == nullptr) {
			delete t;
			return nullptr;
		}
		if (t->ch[0] == nullptr || t->ch[1] == nullptr) {
			node *res = t->ch[t->ch[0] == nullptr];
			delete t;
			return res;
		}
		node *res = new node(find(t->ch[1], 0)->val, t->ch[0], erase(t->ch[1], 0));
		delete t;
		return fetch(update(res));
	}
	node *erase(node *t, int k) {
		if (t == nullptr) return nullptr;
		int c = count(t->ch[0]);
		if (k < c) {
			t->ch[0] = erase(t->ch[0], k);
			t = update(t);
		}
		else if (k > c) {
			t->ch[1] = erase(t->ch[1], k - (c + 1));
			t = update(t);
		}
		else {
			t = erase(t);
		}
		return fetch(t);
	}
	node *find(node *t, int k) {
		if (t == nullptr) return t;
		int c = count(t->ch[0]);
		return k < c ? find(t->ch[0], k) : k == c ? t : find(t->ch[1], k - (c + 1));
	}
	int cnt(node *t, T v) {
		if (t == nullptr) return 0;
		if (t->val < v) return count(t->ch[0]) + 1 + cnt(t->ch[1], v);
		if (t->val == v) return count(t->ch[0]);
		return cnt(t->ch[0], v);
	}
	node *root;
public:
	AVL() : root(nullptr) {}
	void insert(T val) {
		root = insert(root, cnt(root, val), val);
	}
	void erase(int k) {
		root = erase(root, k);
	}
	T select(int k) {
		return find(root, k)->val;
	}
	int count(T val) {
		return cnt(root, val);
	}
};
