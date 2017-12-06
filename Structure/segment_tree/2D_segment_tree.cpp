
// normal segment tree
struct minQ {
	using type = int;
	static type id() { return INT_MAX; }
	static type op(const type& l, const type & r) { return min(l, r); }
};

template <typename M>
class SegmentTree {
	using T = typename M::type;
	const int n;
	vector<T> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
public:
	SegmentTree(int n_) :
		n(size(n_)), data(n * 2, M::id()) {}
	void init(const vector<T>& data_) {
		for (int i = 0; i < (int)data_.size(); i++)
			data[i + n] = data_[i];
		for (int i = n - 1; i >= 0; i--)
			data[i] = M::op(data[i * 2], data[i * 2 + 1]);
	}
	void update(int p, T val) {
		p += n;
		data[p] = val;
		while (p >>= 1) data[p] = M::op(data[p * 2], data[p * 2 + 1]);
	}
	void add(int p, T val) {
		p += n;
		data[p] += val;
		while (p >>= 1) data[p] = M::op(data[p * 2], data[p * 2 + 1]);
	}
	T find(int x) {
		return data[n + x];
	}
	T find(int l, int r) {
		l += n; r += n + 1;
		T res1 = M::id(), res2 = M::id();
		while (l < r) {
			if (l & 1) res1 = M::op(res1, data[l++]);
			if (r & 1) res2 = M::op(data[--r], res2);
			l >>= 1; r >>= 1;
		}
		return M::op(res1, res2);
	}
};

// 2D segment tree
template <typename M>
class SegmentTree2 {
	using T = typename M::type;
	const int h;
	vector<SegmentTree<M>> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
public:
	SegmentTree2(int h_, int w_) :
		h(size(h_)), data(h * 2, SegmentTree<M>(w_)) {}
	void init(const vector<vector<T>>& v) {
		for (int i = 0; i < (int)v.size(); i++) data[i + h].init(v[i]);
		int w = v.front().size();
		vector<T> tmp(w);
		for (int i = h - 1; i > 0; i--) {
			for (int j = 0; j < w; j++) {
				tmp[j] = M::op(data[i * 2].find(j), data[i * 2 + 1].find(j));
			}
			data[i].init(tmp);
		}
	}
	void update(int i, int j, T val) {
		i += h;
		data[i].update(j, val);
		while (i >>= 1) data[i].update(j, M::op(data[i * 2].find(j), data[i * 2 + 1].find(j)));
	}
	void add(int i, int j, T val) {
		i += h;
		data[i].Add(j, val);
		while (i >>= 1) data[i].update(j, M::op(data[i * 2].find(j), data[i * 2 + 1].find(j)));
	}
	T find(int i, int j) {
		return data[i + h].find(j);
	}
	T find(int li, int lj, int ri, int rj) {
		li += h; ri += h + 1;
		T res1 = M::id(), res2 = M::id();
		while (li < ri) {
			if (li & 1) res1 = M::op(res1, data[li++].find(lj, rj));
			if (ri & 1) res2 = M::op(data[--ri].find(lj, rj), res2);
			li >>= 1; ri >>= 1;
		}
		return M::op(res1, res2);
	}
};


// simple 2D segment tree (using less memory)
int data[1 << 23];
int v[1 << 20];

template <typename T>
class SegmentTree2 {
	using func_t = function<T(T, T)>;
	const int h, w;
	const T id;
	func_t merge;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
	T find(int i, int lj, int rj) {
		lj += w, rj += w + 1;
		T res1 = id, res2 = id;
		while (lj < rj) {
			if (lj & 1) res1 = merge(res1, data[i * w * 2 + lj++]);
			if (rj & 1) res2 = merge(data[i * w * 2 + --rj], res2);
			lj >>= 1, rj >>= 1;
		}
		return merge(res1, res2);
	}
public:
	SegmentTree2(int h_, int w_, T id_, func_t merge_) :
		h(size(h_)), w(size(w_)), id(id_), merge(merge_) {
		fill(data, data + (1 << 23), id);
		for (int i = 0; i < h_; i++)
			for (int j = 0; j < w_; j++)
				data[(h + i) * w * 2 + w + j] = v[i * w_ + j];
		for (int i = h * 2 - 1; i >= h; i--)
			for (int j = w - 1; j > 0; j--)
				data[i * w * 2 + j] = merge(data[i * w * 2 + j * 2], data[i * w * 2 + j * 2 + 1]);
		for (int i = h - 1; i > 0; i--)
			for (int j = 1; j < w * 2; j++)
				data[i * w * 2 + j] = merge(data[i * 2 * w * 2 + j], data[(i * 2 + 1) * w * 2 + j]);
	}
	void update(int pi, int pj, T val) {
		pi += h;
		pj += w;
		data[pi * w * 2 + pj] = val;
		int tj = pj;
		while (tj >>= 1) {
			data[pi * w * 2 + tj] = merge(data[pi * w * 2 + tj * 2], data[pi * w * 2 + tj * 2 + 1]);
		}
		while (pi >>= 1) {
			data[pi * w * 2 + pj] = merge(data[(pi * 2) * w * 2 + pj], data[(pi * 2 + 1) * w * 2 + pj]);
			tj = pj;
			while (tj >>= 1) {
				data[pi * w * 2 + tj] = merge(data[pi * w * 2 + tj * 2], data[pi * w * 2 + tj * 2 + 1]);
			}
		}
	}
	T find(int li, int lj, int ri, int rj) {
		li += h, ri += h + 1;
		T res1 = id, res2 = id;
		while (li < ri) {
			if (li & 1) res1 = merge(res1, find(li++, lj, rj));
			if (ri & 1) res2 = merge(find(--ri, lj, rj), res2);
			li >>= 1, ri >>= 1;
		}
		return merge(res1, res2);
	}
};
