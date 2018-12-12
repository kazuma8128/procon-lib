
template <typename T>
T power(T x, long long n) {
	T res = 1;
	while (n) {
		if (n & 1) res *= x;
		x *= x;
		n >>= 1;
	}
	return res;
}

template <typename T>
T mod_inv(T a, T md) {
	T b = md, u = 1, v = 0;
	while (b != 0) {
		T t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	u %= md;
	if (u < 0) u += md;
	return u;
}

template <typename T>
vector<pair<T, int>> factorize(T n) {
	vector<pair<T, int>> res;
	for (T i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			res.emplace_back(i, 0);
			while (n % i == 0) {
				res.back().second++;
				n /= i;
			}
		}
	}
	if (n != 1) {
		if (!res.empty() && res.back().first == n) res.back().second++;
		else res.emplace_back(n, 1);
	}
	return res;
}

template <typename T>
T chinese_remainder_theorem(const vector<pair<T, T>>& cs, T md) {
	unordered_map<T, pair<int, T>> facts;
	for (auto cc : cs) {
		T x, m;
		tie(x, m) = cc;
		for (auto& pp : factorize(m)) {
			T p; int k;
			tie(p, k) = pp;
			if (!facts.count(p)) {
				facts[p] = make_pair(k, x % power(p, k));
				continue;
			}
			auto q = facts[p];
			if ((x - q.second) % power(p, min(k, q.first)) != 0) return -1;
			if (k > q.first) {
				facts[p] = make_pair(k, x % power(p, k));
			}
		}
	}
	bool zero = true;
	vector<pair<T, T>> mr;
	for (auto p : facts) {
		mr.emplace_back(power(p.first, p.second.first), p.second.second);
		zero &= (p.second.second == 0);
	}
	if (zero) {
		T res = 1;
		for (auto p : facts) {
			(res *= power(p.first, p.second.first)) %= md;
		}
		return res;
	}
	mr.emplace_back(md, 0);
	int n = mr.size();
	vector<T> coffs(n, 1);
	vector<T> constants(n, 0);
	for (int i = 0; i < n - 1; i++) {
		T v = (mr[i].second - constants[i]) * mod_inv(coffs[i], mr[i].first) % mr[i].first;
		if (v < 0) v += mr[i].first;
		for (int j = i + 1; j < n; j++) {
			(constants[j] += coffs[j] * v) %= mr[j].first;
			(coffs[j] *= mr[i].first) %= mr[j].first;
		}
	}
	return constants.back();
}
