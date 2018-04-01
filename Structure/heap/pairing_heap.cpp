
// pairing heap
template <typename T>
struct h_node {
	T val;
	vector<h_node<T>*> chi;
	h_node() : chi() {}
	h_node(int v) : val(v), chi() {}
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
h_node<T>* merge(h_node<T>* a, h_node<T>* b) {
	if (a == nullptr) return b;
	if (b == nullptr) return a;
	if (a->val > b->val) swap(a, b);	// if maximum heap, change to <=
	a->chi.push_back(b);
	return a;
}

template <typename T>
h_node<T>* sub_merge(const vector<h_node<T>*>& p) {
	int n = p.size();
	vector<h_node<T>*> tmp;
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
	h_node<T> *res = tmp[0];
	for (int i = 1; i < n; i++) {
		res = merge(res, tmp[i]);
	}
	return res;
}
