
// normal ver
template <typename T>
struct h_node {
	h_node *l, *r;
	T val;
	h_node() {}
	h_node(int v) : l(nullptr), r(nullptr), val(v) {}
	void init(int v) {
		l = nullptr; r = nullptr; val = v;
	}
};

template <typename T>
h_node<T>* meld(h_node<T>* a, h_node<T>* b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	if (a->val < b->val) swap(a, b); // if minimum h_node, >
	a->r = meld(a->r, b);
	swap(a->l, a->r);
	return a;
}

template <typename T>
h_node<T>* pop(h_node<T>* a) {
	return meld(a->l, a->r);
}

h_node<int> pool[2000000];
int it;


// class ver
template <typename T>
struct h_node {
	h_node *l, *r;
	T val;
	h_node() {}
	h_node(int v) : l(nullptr), r(nullptr), val(v) {}
	init(int v) {
		l = nullptr; r = nullptr; val = v;
	}
};

h_node<int> pool[2000000];
int it;
class skew_heap {
	h_node<int>* root;
	h_node<int>* meld(h_node<int>* a, h_node<int>* b) {
		if (a == nullptr) return b;
		if (b == nullptr) return a;
		if (a->val < b->val) swap(a, b); // if minimum heap, >
		a->r = meld(a->r, b);
		swap(a->l, a->r);
		return a;
	}
	h_node<int>* pop_(h_node<int>* a) {
		return meld(a->l, a->r);
	}
public:
	skew_heap() : root(nullptr) {}
	void push(int val) {
		pool[it].init(val);
		root = meld(root, &pool[it++]);
	}
	int top() {
		return root->val;
	}
	void pop() {
		root = pop_(root);
	}
	void meld(h_node<int>* a) {
		root = meld(root, a);
	}
	void clear() {
		root = nullptr;
	}
};
