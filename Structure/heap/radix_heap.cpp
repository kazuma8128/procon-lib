
// for speed-up of dijkstra
// cost : int
template <class T>
class radix_heap_32 {
	int bsr(unsigned x) {
		return x ? 31 - __builtin_clz(x) : -1;
	}
	vector<pair<int, T>> v[33];
	int last, sz;
public:
	radix_heap_32() : last(0), sz(0) {}
	bool empty() const {
		return !sz;
	}
	void push(int x, T val) {
		assert(last <= x);
		++sz;
		v[bsr(x ^ last) + 1].emplace_back(x, val);
	}
	pair<int, T> pop() {
		assert(sz);
		if (v[0].empty()) {
			int i = 1;
			while (v[i].empty()) ++i;
			pair<int, T> new_last = *min_element(v[i].begin(), v[i].end());
			for (auto x : v[i])  v[bsr(x.first ^ new_last.first) + 1].push_back(x);
			last = new_last.first;
			v[i].clear();
		}
		--sz;
		auto res = v[0].back(); v[0].pop_back();
		return res;
	}
};

// cost : long long
template <class T>
class radix_heap_64 {
	int bsr(unsigned long long x) {
		return x ? 63 - __builtin_clzll(x) : -1;
	}
	vector<pair<long long, T>> v[65];
	long long last;
	int sz;
public:
	radix_heap_64() : last(0), sz(0) {}
	bool empty() const {
		return !sz;
	}
	void push(long long x, T val) {
		assert(last <= x);
		++sz;
		v[bsr(x ^ last) + 1].emplace_back(x, val);
	}
	pair<long long, T> pop() {
		assert(sz);
		if (v[0].empty()) {
			int i = 1;
			while (v[i].empty()) ++i;
			pair<long long, T> new_last = *min_element(v[i].begin(), v[i].end());
			for (auto x : v[i])  v[bsr(x.first ^ new_last.first) + 1].push_back(x);
			last = new_last.first;
			v[i].clear();
		}
		--sz;
		auto res = v[0].back(); v[0].pop_back();
		return res;
	}
};
