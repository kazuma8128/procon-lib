
class fid {
	int n;
	vector<bool> data;
	vector<int> ra, se0, se1;
public:
	fid(int n_) : n(n_), data(n), ra(n + 1) {}
	void set(int i) {
		data[i] = true;
	}
	void build() {
		for (int i = 0; i < n; i++) {
			ra[i + 1] = ra[i] + data[i];
		}
		for (int i = 0; i < n; i++) {
			(data[i] ? se1 : se0).push_back(i);
		}
	}
	int rank(int i, bool b) const {
		return b ? ra[i] : i - ra[i];
	}
	int rank(int l, int r, bool b) const {
		return rank(r, b) - rank(l, b);
	}
	int select(int x, bool b) const {
		return (b ? se1 : se0)[x];
	}
	int select(int l, int x, bool b) const {
		return select(x + rank(l, b), b);
	}
};

template <typename T>
class wavelet_matrix {
	T h;
	int n;
	vector<fid> data;
	vector<int> mid;

	T get_h(T val) {
		T res = 1;
		while ((1LL << res) <= val) ++res;
		return res;
	}
public:
	wavelet_matrix(const vector<T>& data_)
		: h(get_h(*max_element(data_.begin(), data_.end()))), n(data_.size()), mid(h) {
		data.assign(h, n);
		vector<T> ar1(data_), ar2(n);
		for (T b = 0; b < h; b++) {
			int p = 0;
			for (int i = 0; i < n; i++) {
				if ((ar1[i] & ((T)1 << (h - 1 - b))) == 0) {
					ar2[p++] = ar1[i];
				}
			}
			mid[b] = p;
			for (int i = 0; i < n; i++) {
				if (ar1[i] & ((T)1 << (h - 1 - b))) {
					data[b].set(i);
					ar2[p++] = ar1[i];
				}
			}
			data[b].build();
			ar1.swap(ar2);
		}
	}
	int rank(int p, T val) {
		return rank(0, p, val);
	}
	int rank(int l, int r, T val) {
		if (val >> h) return 0;
		for (T b = 0; b < h; b++) {
			if (val & ((T)1 << (h - 1 - b))) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		return r - l;
	}
	int rank_less_than(int l, int r, T ub) {
		if (ub >> h) return r - l;
		int res = 0;
		for (T b = 0; b < h; b++) {
			bool d = (ub >> (h - 1 - b)) & 1;
			int lcnt = data[b].rank(l, d);
			int rcnt = data[b].rank(r, d);
			if (d) res += (r - l) - (rcnt - lcnt);
			l = lcnt;
			r = rcnt;
			if (d) {
				l += mid[b];
				r += mid[b];
			}
		}
		return res;
	}
	int rank_range(int l, int r, T lb, T ub) {
		return rank_less_than(l, r, ub) - rank_less_than(l, r, lb);
	}
	int select(int x, T val) {
		static int left[h];
		int l = 0, r = n;
		for (T b = 0; b < h; b++) {
			left[b] = l;
			if (val & ((T)1 << (h - 1 - b))) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		for (int b = h - 1; b >= 0; b--) {
			x = data[b].select(left[b], x, (bool)((val >> (h - 1 - b)) & 1)) - left[b];
		}
		return x;
	}
	int select(int l, int r, int x, T val) {
		return select(x + rank(l, val), val);
	}
	T kth_element(int l, int r, int k) {
		T res = 0;
		for (T b = 0; b < h; b++) {
			int cnt = data[b].rank(l, r, false);
			res <<= 1;
			if (k >= cnt) {
				l = data[b].rank(l, true) + mid[b];
				r = data[b].rank(r, true) + mid[b];
				k -= cnt;
				res |= 1;
			}
			else {
				l = data[b].rank(l, false);
				r = data[b].rank(r, false);
			}
		}
		return res;
	}
};
