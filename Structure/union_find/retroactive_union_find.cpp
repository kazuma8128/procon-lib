
class retroactive_uf {
	vector<int> data;
	vector<pair<int, int>> hist;
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
		hist.emplace_back(b, data[b]);
		data[b] = data[a] + data[b];
		hist.emplace_back(a, data[a]);
		data[a] = b;
	}
	int size(int a) const {
		return -data[find(a)];
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
