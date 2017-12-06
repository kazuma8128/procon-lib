
using ll = long long;

const function<int(int, int)> sumQ = [](int l, int r) { return l + r; };
const function<int(int, int)> minQ = [](int l, int r) { return min(l, r); };
const function<int(int, int)> maxQ = [](int l, int r) { return max(l, r); };
const function<ll(ll, ll)> sumQll = [](ll l, ll r) { return l + r; };
const function<ll(ll, ll)> minQll = [](ll l, ll r) { return min(l, r); };
const function<ll(ll, ll)> maxQll = [](ll l, ll r) { return max(l, r); };

// merge sort tree (?)
template <typename T>
class MergeTree {
	const int n;
	vector<vector<T>> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
	int query(int node, T val) {	// change for each query
		return upper_bound(data[node].begin(), data[node].end(), val) - data[node].begin();
	}
public:
	MergeTree(const vector<T>& data_) : n(size(data_.size())), data(n * 2) {
		for (int i = 0; i < (int)data_.size(); i++)
			data[i + n].push_back(data_[i]);
		for (int i = n - 1; i >= 0; i--)
			merge(data[i * 2].begin(), data[i * 2].end(), data[i * 2 + 1].begin(), data[i * 2 + 1].end(), back_inserter(data[i]));
	}
	int find(int l, int r, T val) {
		l += n; r += n + 1;
		int res = 0;
		while (l < r) {
			if (l & 1) res += query(l++, val);
			if (r & 1) res += query(--r, val);
			l >>= 1; r >>= 1;
		}
		return res;
	}
};

// a little generalized
template <typename T>
class MergeTree {
	const int n;
	vector<vector<T>> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
public:
	MergeTree(const vector<T>& data_) : n(size(data_.size())), data(n * 2) {
		for (int i = 0; i < (int)data_.size(); i++) {
			data[i + n].push_back(data_[i]);
		}
		for (int i = n - 1; i >= 0; i--) {
			merge(data[i * 2].begin(), data[i * 2].end(), data[i * 2 + 1].begin(), data[i * 2 + 1].end(), back_inserter(data[i]));
		}
	}
	int find(int l, int r, function<int(const vector<T>&)> que) {
		l += n; r += n + 1;
		int res = 0;
		while (l < r) {
			if (l & 1) res += que(data[l++]);
			if (r & 1) res += que(data[--r]);
			l >>= 1; r >>= 1;
		}
		return res;
	}
};

// generalized, but slow
template <typename T>
class MergeTree {
	const int n;
	vector<vector<T>> data;
	int size(int n) {
		int res;
		for (res = 1; res < n; res <<= 1);
		return res;
	}
public:
	MergeTree(const vector<T>& data_) : n(size(data_.size())), data(n * 2)
	{
		for (int i = 0; i < (int)data_.size(); i++) {
			data[i + n].push_back(data_[i]);
		}
		for (int i = n - 1; i >= 0; i--) {
			merge(data[i * 2].begin(), data[i * 2].end(), data[i * 2 + 1].begin(), data[i * 2 + 1].end(), back_inserter(data[i]));
		}
	}
	void for_each(int l, int r, function<void(vector<T>&)> que) {
		l += n; r += n + 1;
		while (l < r) {
			if (l & 1) que(data[l++]);
			if (r & 1) que(data[--r]);
			l >>= 1; r >>= 1;
		}
	}
	void for_each_ordered(int l, int r, function<void(vector<T>&)> que) {
		l += n; r += n + 1;
		vector<int> v;
		while (l < r) {
			if (l & 1) que(data[l++]);
			if (r & 1) v.push_back(--r);
			l >>= 1; r >>= 1;
		}
		reverse(v.begin(), v.end());
		for (auto node : v) {
			que(data[node]);
		}
	}
};
