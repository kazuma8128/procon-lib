
// pairing heap
template <typename T>
struct node {
	T val;
	vector<node<T>*> chi;
	node() : chi() {}
	node(int v) : val(v), chi() {}
	void init(int v) {
		val = v;
	}
	void erase() {
		for (auto p : chi) {
			p->erase();
			delete p;
		}
	}
};

template <typename T>
node<T>* merge(node<T>* a, node<T>* b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	if (a->val > b->val) swap(a, b);	// if maximum heap, change to <=
	a->chi.push_back(b);
	return a;
}

template <typename T>
node<T>* sub_merge(const vector<node<T>*>& p) {
	int n = p.size();
	vector<node<T>*> tmp;
	for (int i = 0; i < n; i += 2) {
		if (i + 1 < n) {
			tmp.push_back(merge(p[i], p[i + 1]));
		}
		else {
			tmp.push_back(p[i]);
		}
	}
	reverse(tmp.begin(), tmp.end());
	n = tmp.size();
	node<T> *res = tmp[0];
	for (int i = 1; i < n; i++) {
		res = merge(res, tmp[i]);
	}
	return res;
}
