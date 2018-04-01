
// normal (not fast)
template <typename T>
vector<T> inc_kitamasa(const vector<T>& tmp, const vector<T>& b) {
	int k = tmp.size();
	vector<T> res(k);
	for (int i = 0; i < k; i++) {
		res[i] = tmp[k - 1] * b[i] + (i - 1 >= 0 ? tmp[i - 1] : 0);
	}
	return res;
}

template <typename T>
vector<T> dbl_kitamasa(const vector<T>& tmp, const vector<T>& b) {
	int k = tmp.size();
	vector<vector<T>> table(k);
	table[0] = tmp;
	for (int i = 1; i < k; i++) {
		table[i] = inc_kitamasa(table[i - 1], b);
	}
	vector<T> res(k);
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			res[j] += table[i][j] * tmp[i];
		}
	}
	return res;
}

template <typename T>
vector<T> kitamasa(const vector<T>& b, unsigned n) {
	int k = b.size();
	vector<T> res(k); res[0] = 1;
	if (!n) return res;
	for (int i = 31 - __builtin_clz(n); i >= 0; i--) {
		res = dbl_kitamasa(res, b);
		if (n & (1 << i)) {
			res = inc_kitamasa(res, b);
		}
	}
	return res;
}
