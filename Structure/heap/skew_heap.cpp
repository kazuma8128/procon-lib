
template <typename T>
class skew_heap {
	struct node {
		node *l, *r;
		T val;
		node(T v) : l(nullptr), r(nullptr), val(v) {}
	};
	node* meld(node* a, node* b) {
		if (!a) return b;
		if (!b) return a;
		if (a->val < b->val) swap(a, b); // min heap : >
		a->r = meld(a->r, b);
		swap(a->l, a->r);
		return a;
	}
	node *root;
public:
	skew_heap() : root(nullptr) {}
	bool empty() const {
		return !root;
	}
	T top() const {
		return root->val;
	}
	void pop() {
		root = meld(root->l, root->r);
	}
	void push(T val) {
		root = meld(root, new node(val));
	}
	void meld(skew_heap<T>& that) {
		root = meld(root, that.root);
		that.root = nullptr;
	}
};
