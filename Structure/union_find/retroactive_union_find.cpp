
template<typename T>
class retroactive_array {
	vector<T> data;
	vector<pair<int, T>> hist;
public:
	retroactive_array(int N) : data(N) {}
	retroactive_array(int N, T val) : data(N, val) {}
	retroactive_array(const vector<T>& v) : data(v) {}
	retroactive_array(vector<T>&& v) : data(move(v)) {}
	void change(int k, T val) {
		hist.emplace_back(k, data[k]);
		data[k] = val;
	}
	T operator[](int k) const {
		return data[k];
	}
	int get_version() const {
		return hist.size();
	}
	void rollback(int ver) {
		assert(0 <= ver && ver <= (int)hist.size());
		int cnt = hist.size() - ver;
		while (cnt--) {
			data[hist.back().first] = hist.back().second;
			hist.pop_back();
		}
	}
};

class retroactive_uf {
	retroactive_array<int> data;
public:
	retroactive_uf(int n) : data(n, -1) {}
	int find(int a) const {
		return data[a] < 0 ? a : find(data[a]);
	}
	bool same(int a, int b) const {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (-data[a] > -data[b]) swap(a, b);
		data.change(b, data[a] + data[b]);
		data.change(a, b);
	}
	int size(int a) const {
		return -data[find(a)];
	}
	int get_version() const {
		return data.get_version();
	}
	void rollback(int ver) {
		data.rollback(ver);
	}
};
