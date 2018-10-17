
template <typename T>
class pairing_heap {
	struct node {
		T val;
		vector<node*> chi;
		node(T v) : val(v) {}
	};
	node* meld(node* a, node* b) {
		if (!a) return b;
		if (!b) return a;
		if (a->val < b->val) swap(a, b); // min heap : >
		a->chi.push_back(b);
		return a;
	}
	node* sub_meld(const vector<node*>& p) {
		if (p.empty()) return nullptr;
		int n = p.size();
		vector<node*> tmp;
		for (int i = 0; i < n; i += 2) {
			if (i + 1 < n) {
				tmp.push_back(meld(p[i], p[i + 1]));
			}
			else {
				tmp.push_back(p[i]);
			}
		}
		reverse(tmp.begin(), tmp.end());
		n = tmp.size();
		node *res = tmp[0];
		for (int i = 1; i < n; i++) {
			res = meld(res, tmp[i]);
		}
		return res;
	}
	node *root;
public:
	pairing_heap() : root(nullptr) {}
	T top() const {
		return root->val;
	}
	bool empty() const {
		return !root;
	}
	void pop() {
		node *tmp = sub_meld(root->chi);
		delete root;
		root = tmp;
	}
	void push(T val) {
		root = meld(root, new node(val));
	}
	void meld(pairing_heap<T>& that) {
		root = meld(root, that.root);
		that.root = nullptr;
	}
};
