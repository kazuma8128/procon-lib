
// normal ver
template <typename T>
struct Heap {
	Heap *l, *r;
	T val;
	Heap() {}
	Heap(int v) : l(nullptr), r(nullptr), val(v) {}
	void init(int v) {
		l = nullptr; r = nullptr; val = v;
	}
};

template <typename T>
Heap<T>* meld(Heap<T>* a, Heap<T>* b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	if (a->val < b->val) swap(a, b); // if minimum heap, >
	a->r = meld(a->r, b);
	swap(a->l, a->r);
	return a;
}

template <typename T>
Heap<T>* pop(Heap<T>* a) {
	return meld(a->l, a->r);
}

Heap<int> pool[2000000];
int it;


// class ver
template <typename T>
struct node {
	node *l, *r;
	T val;
	node() {}
	node(int v) : l(nullptr), r(nullptr), val(v) {}
	init(int v) {
		l = nullptr; r = nullptr; val = v;
	}
};

node<int> pool[2000000];
int it;
class Priority_Queue {
	node<int>* root;
	node<int>* meld(node<int>* a, node<int>* b) {
		if (a == nullptr) return b;
		if (b == nullptr) return a;
		if (a->val < b->val) swap(a, b); // if minimum heap, >
		a->r = meld(a->r, b);
		swap(a->l, a->r);
		return a;
	}
	node<int>* pop_(node<int>* a) {
		return meld(a->l, a->r);
	}
public:
	Priority_Queue() : root(nullptr) {}
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
	void meld(node<int>* a) {
		root = meld(root, a);
	}
	void clear() {
		root = nullptr;
	}
};
