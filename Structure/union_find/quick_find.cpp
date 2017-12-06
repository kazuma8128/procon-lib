
class QuickFind {
	vector<int> rt;
	vector<vector<int>> vs;
public:
	QuickFind(int n) : rt(n), vs(n) {
		for (int i = 0; i < n; i++) rt[i] = i, vs[i].push_back(i);
	}
	bool same(int a, int b) {
		return rt[a] == rt[b];
	}
	void unite(int a, int b) {
		a = rt[a], b = rt[b];
		if (a == b) return;
		if (vs[a].size() < vs[b].size()) swap(a, b);
		for (auto& v : vs[b]) rt[v] = a, vs[a].push_back(v);
	}
	int size(int a) {
		return vs[rt[a]].size();
	}
};
