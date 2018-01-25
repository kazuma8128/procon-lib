
// space complexity : O(max(cnt, √U))
template <const int U = 1 << 30>
class vEB {

	const int ctz;

	struct node {
		int min, max;
		node *summary, **cluster;
		node() : min(U), max(-1), summary(nullptr), cluster(nullptr) {}
	} *root;

	int cnt;

private:

	int downsqrt(int ub) const {
		return ub >> 1;
	}

	int upsqrt(int ub) const {
		return (ub + 1) >> 1;
	}

	int high(int x, int ub) const {
		return x >> downsqrt(ub);
	}

	int low(int x, int ub) const {
		return x & ((1 << downsqrt(ub)) - 1);
	}

	int index(int x, int y, int ub) const {
		return (x << downsqrt(ub)) + y;
	}

	int minimum(node *v) const {
		return v ? v->min : U;
	}

	int maximum(node *v) const {
		return v ? v->max : -1;
	}

	bool member(node *v, int x, int ub) const {
		if (!v) return false;
		if (v->min == x || v->max == x) return true;
		if (!v->cluster) return false;
		return member(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
	}

	node* new_node(int ub) {
		node *v = new node();
		if (ub > 1) {
			int sz = 1 << upsqrt(ub);
			v->cluster = new node*[sz];
			for (int i = 0; i < sz; i++) {
				v->cluster[i] = nullptr;
			}
		}
		return v;
	}

	node* empty_insert(node *v, int x, int ub) {
		if (!v) v = new_node(ub);
		v->min = v->max = x;
		return v;
	}

	node* insert(node *v, int x, int ub) {
		if (!v) v = new_node(ub);
		if (v->min == U) {
			empty_insert(v, x, ub);
			return v;
		}
		if (x < v->min) swap(x, v->min);
		if (ub > 1) {
			if (minimum(v->cluster[high(x, ub)]) == U) {
				v->summary = insert(v->summary, high(x, ub), upsqrt(ub));
				v->cluster[high(x, ub)] = empty_insert(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
			}
			else {
				v->cluster[high(x, ub)] = insert(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
			}
		}
		if (x > v->max) v->max = x;
		return v;
	}

	node* erase(node *v, int x, int ub) {
		if (v->min == v->max) {
			v->min = U;
			v->max = -1;
			return nullptr;
		}
		if (ub <= 1) {
			v->max = v->min = (x == 0);
			return v;
		}
		if (x == v->min) {
			int first_cluster = minimum(v->summary);
			v->min = x = index(first_cluster, minimum(v->cluster[first_cluster]), ub);
		}
		v->cluster[high(x, ub)] = erase(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
		if (minimum(v->cluster[high(x, ub)]) == U) {
			v->summary = erase(v->summary, high(x, ub), upsqrt(ub));
			if (x == v->max) {
				int summary_max = maximum(v->summary);
				if (summary_max == -1) {
					v->max = v->min;
				}
				else {
					v->max = index(summary_max, maximum(v->cluster[summary_max]), ub);
				}
			}
		}
		else if (x == v->max) {
			v->max = index(high(x, ub), maximum(v->cluster[high(x, ub)]), ub);
		}
		return v;
	}

	int successor(node *v, int x, int ub) const {
		if (!v) return U;
		if (ub <= 1) {
			if (x == 0 && v->max == 1) return 1;
			return U;
		}
		if (v->min != U && x < v->min) return v->min;
		int max_low = maximum(v->cluster[high(x, ub)]);
		if (max_low != -1 && low(x, ub) < max_low) {
			return index(high(x, ub), successor(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub)), ub);
		}
		int succ_cluster = successor(v->summary, high(x, ub), upsqrt(ub));
		if (succ_cluster == U) return U;
		return index(succ_cluster, minimum(v->cluster[succ_cluster]), ub);
	}

	int predecessor(node *v, int x, int ub) const {
		if (!v) return -1;
		if (ub <= 1) {
			if (x == 1 && v->min == 0) return 0;
			return -1;
		}
		if (v->max != -1 && x > v->max) return v->max;
		int min_low = minimum(v->cluster[high(x, ub)]);
		if (min_low != U && low(x, ub) > min_low) {
			return index(high(x, ub), predecessor(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub)), ub);
		}
		int pred_cluster = predecessor(v->summary, high(x, ub), upsqrt(ub));
		if (pred_cluster == -1) {
			if (v->min != U && x > v->min) return v->min;
			return -1;
		}
		return index(pred_cluster, maximum(v->cluster[pred_cluster]), ub);
	}

public:

	vEB() : ctz(__builtin_ctz(U)), root(nullptr), cnt(0) {}

	int size() const {
		return cnt;
	}

	bool member(int x) const {
		return member(root, x, ctz);
	}

	int minimum() const {
		return minimum(root);
	}

	int maximum() const {
		return maximum(root);
	}

	void insert(int x) {
		if (!member(x)) root = insert(root, x, ctz), cnt++;
	}

	void erase(int x) {
		if (member(x)) root = erase(root, x, ctz), cnt--;
	}

	int successor(int x) const {
		return successor(root, x, ctz);
	}

	int predecessor(int x) const {
		return predecessor(root, x, ctz);
	}

};

// space complexity : O(cnt)
template <typename T = int, const T U = (T)1 << (T)30>
class vEB {

	const T ctz;

	struct node {
		T min, max;
		node *summary;
		unordered_map<T, node*> cluster;
		node() : min(U), max(-1), summary(nullptr) {}
	} *root;

	int cnt;

private:

	T downsqrt(T ub) const {
		return ub >> (T)1;
	}

	T upsqrt(T ub) const {
		return (ub + 1) >> (T)1;
	}

	T high(T x, T ub) const {
		return x >> downsqrt(ub);
	}

	T low(T x, T ub) const {
		return x & (((T)1 << downsqrt(ub)) - 1);
	}

	T index(T x, T y, T ub) const {
		return (x << downsqrt(ub)) + y;
	}

	T minimum(node *v) const {
		return v ? v->min : U;
	}

	T maximum(node *v) const {
		return v ? v->max : -1;
	}

	bool member(node *v, T x, T ub) const {
		if (!v) return false;
		if (v->min == x || v->max == x) return true;
		if (v->cluster.empty()) return false;
		return member(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
	}

	node* empty_insert(node *v, T x, T ub) {
		if (!v) v = new node();
		v->min = v->max = x;
		return v;
	}

	node* insert(node *v, T x, T ub) {
		if (!v) v = new node();
		if (v->min == U) {
			empty_insert(v, x, ub);
			return v;
		}
		if (x < v->min) swap(x, v->min);
		if (ub > 1) {
			if (minimum(v->cluster[high(x, ub)]) == U) {
				v->summary = insert(v->summary, high(x, ub), upsqrt(ub));
				v->cluster[high(x, ub)] = empty_insert(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
			}
			else {
				v->cluster[high(x, ub)] = insert(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
			}
		}
		if (x > v->max) v->max = x;
		return v;
	}

	node* erase(node *v, T x, T ub) {
		if (v->min == v->max) {
			v->min = U;
			v->max = -1;
			return nullptr;
		}
		if (ub <= 1) {
			v->max = v->min = (x == 0);
			return v;
		}
		if (x == v->min) {
			T first_cluster = minimum(v->summary);
			v->min = x = index(first_cluster, minimum(v->cluster[first_cluster]), ub);
		}
		v->cluster[high(x, ub)] = erase(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub));
		if (minimum(v->cluster[high(x, ub)]) == U) {
			v->summary = erase(v->summary, high(x, ub), upsqrt(ub));
			if (x == v->max) {
				T summary_max = maximum(v->summary);
				if (summary_max == -1) {
					v->max = v->min;
				}
				else {
					v->max = index(summary_max, maximum(v->cluster[summary_max]), ub);
				}
			}
		}
		else if (x == v->max) {
			v->max = index(high(x, ub), maximum(v->cluster[high(x, ub)]), ub);
		}
		return v;
	}

	T successor(node *v, T x, T ub) const {
		if (!v) return U;
		if (ub <= 1) {
			if (x == 0 && v->max == 1) return 1;
			return U;
		}
		if (v->min != U && x < v->min) return v->min;
		T max_low = maximum(v->cluster[high(x, ub)]);
		if (max_low != -1 && low(x, ub) < max_low) {
			return index(high(x, ub), successor(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub)), ub);
		}
		T succ_cluster = successor(v->summary, high(x, ub), upsqrt(ub));
		if (succ_cluster == U) return U;
		return index(succ_cluster, minimum(v->cluster[succ_cluster]), ub);
	}

	T predecessor(node *v, T x, T ub) const {
		if (!v) return -1;
		if (ub <= 1) {
			if (x == 1 && v->min == 0) return 0;
			return -1;
		}
		if (v->max != -1 && x > v->max) return v->max;
		T min_low = minimum(v->cluster[high(x, ub)]);
		if (min_low != U && low(x, ub) > min_low) {
			return index(high(x, ub), predecessor(v->cluster[high(x, ub)], low(x, ub), downsqrt(ub)), ub);
		}
		T pred_cluster = predecessor(v->summary, high(x, ub), upsqrt(ub));
		if (pred_cluster == -1) {
			if (v->min != U && x > v->min) return v->min;
			return -1;
		}
		return index(pred_cluster, maximum(v->cluster[pred_cluster]), ub);
	}

	T get_ctz(T x) {
		T tmp = 1, res = 0;
		while (tmp < x) {
			tmp <<= (T)1;
			res++;
		}
		return res;
	}

public:

	vEB() : ctz(get_ctz(U)), root(nullptr), cnt(0) {}

	int size() const {
		return cnt;
	}

	bool member(T x) const {
		return member(root, x, ctz);
	}

	T minimum() const {
		return minimum(root);
	}

	T maximum() const {
		return maximum(root);
	}

	void insert(T x) {
		if (!member(x)) root = insert(root, x, ctz), cnt++;
	}

	void erase(T x) {
		if (member(x)) root = erase(root, x, ctz), cnt--;
	}

	T successor(T x) const {
		return successor(root, x, ctz);
	}

	T predecessor(T x) const {
		return predecessor(root, x, ctz);
	}

};
