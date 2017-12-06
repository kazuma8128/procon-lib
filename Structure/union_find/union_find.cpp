
// rank ver
class UnionFind {
	vector<int> par;
	int find(int a) {
		return par[a] < 0 ? a : par[a] = find(par[a]);
	}
public:
	UnionFind(int n) : par(n, -1) {}
	bool same(int a, int b) {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (par[a] < par[b]) swap(a, b);
		else if (par[a] == par[b]) par[b]--;
		par[a] = b;
	}
};

// rank ver, able to get size
class UnionFind {
	vector<int> par;
	vector<int> rank;
	int find(int a) {
		return par[a] < 0 ? a : par[a] = find(par[a]);
	}
public:
	UnionFind(int n) : par(n, -1), rank(n, 1) {}
	bool same(int a, int b) {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (rank[a] > rank[b]) {
			par[a] += par[b];
			par[b] = a;
		}
		else {
			par[b] += par[a];
			par[a] = b;
			if (rank[a] == rank[b]) rank[b]++;
		}
	}
	int size(int a) {
		return -par[find(a)];
	}
};

// size ver
class UnionFind {
	vector<int> par;
	int find(int a) {
		return par[a] < 0 ? a : par[a] = find(par[a]);
	}
public:
	UnionFind(int n) : par(n, -1) {}
	bool same(int a, int b) {
		return find(a) == find(b);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (par[a] < par[b]) {
			par[a] += par[b];
			par[b] = a;
		}
		else {
			par[b] += par[a];
			par[a] = b;
		}
	}
	int size(int a) {
		return -par[find(a)];
	}
};
