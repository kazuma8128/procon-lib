
// speed-up by fractional-cascading
template <typename T>
class LayerdRangeTree {
	const int n;
	vector<vector<T>> data;
	vector<vector<int>> tol, tor;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
	int sub(int l, int r, int left, int right, int node, int lb, int ub) {
		if (ub <= l || r <= lb) return 0;
		if (l <= lb && ub <= r) return right - left;
		return sub(l, r, tol[node][left], tol[node][right], node * 2, lb, (lb + ub) / 2)
			+ sub(l, r, tor[node][left], tor[node][right], node * 2 + 1, (lb + ub) / 2, ub);
	}
	void merge(int node) {
		int ln = node * 2, rn = node * 2 + 1;
		int ll = data[ln].size(), rl = data[rn].size();
		for (int i = 0, j = 0; i < ll || j < rl;) {
			tol[node].push_back(i), tor[node].push_back(j);
			if (i == ll) {
				data[node].push_back(data[rn][j++]);
			}
			else if (j == rl) {
				data[node].push_back(data[ln][i++]);
			}
			else if (data[ln][i] <= data[rn][j]) {
				data[node].push_back(data[ln][i++]);
			}
			else {
				data[node].push_back(data[rn][j++]);
			}
		}
		tol[node].push_back(ll), tor[node].push_back(rl);
	}
public:
	LayerdRangeTree(const vector<T>& data_) : n(size(data_.size())), data(n * 2), tol(n), tor(n) {
		for (int i = 0; i < (int)data_.size(); i++)
			data[i + n].push_back(data_[i]);
		for (int i = n - 1; i >= 0; i--) {
			int ns = data[i * 2].size() + data[i * 2 + 1].size();
			data[i].reserve(ns);
			tol[i].reserve(ns + 1);
			tor[i].reserve(ns + 1);
			merge(i);
		}
	}
	int rank(int l, int r, T low, T up) {
		int lb = lower_bound(data[1].begin(), data[1].end(), low) - data[1].begin();
		int ub = lower_bound(data[1].begin(), data[1].end(), up) - data[1].begin();
		return sub(l, r + 1, lb, ub, 1, 0, n);
	}
};
