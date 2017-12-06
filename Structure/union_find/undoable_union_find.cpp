
// undoable array
template<typename T>
class rArray {
	int N;
	vector<int> pos, pre, data;
public:
	rArray(int N) : N(N), data(N) {}
	rArray(int N, T val) : N(N), data(N, val) {}
	rArray(const vector<T>& v) : N(v.size()), data(v) {}
	rArray(vector<T>&& v) : N(v.size()), data(move(v)) {}
	T change(int k, T val) {
		pos.push_back(k);
		pre.push_back(data[k]);
		return data[k] = val;
	}
	void undo() {
		data[pos.back()] = pre.back();
		pos.pop_back(), pre.pop_back();
	}
	T operator[](int k) const {
		return data[k];
	}
	int getversion() const {
		return pos.size();
	}
};

// undoable union find
class rUnionFind {
	rArray<int> data;
	vector<int> pre;
	int find(int a) {
		return data[a] < 0 ? a : data.change(a, find(data[a]));
	}
public:
	rUnionFind(int n) : data(n, -1) {}
	bool same(int a, int b) {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		pre.push_back(data.getversion());
		if (data[a] < data[b]) swap(a, b);
		data.change(b, data[a] + data[b]);
		data.change(a, b);
	}
	void undo() {
		int cnt = data.getversion() - pre.back();
		while (cnt--) data.undo();
		pre.pop_back();
	}
	int size(int a) {
		return -data[find(a)];
	}
	int getversion() {
		return pre.size();
	}
};
