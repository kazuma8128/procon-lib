
const int var = 26;	// the number of charactor types

int trans(char c) {
	return c - 'a';
}

class aho_corasick {
	struct ac_node {
		int fail;
		vector<int> next;
		vector<int> ok;
		ac_node() : fail(-1), next(var, -1) {}
	};
	vector<int> unite(const vector<int>& a, const vector<int>& b) {
		vector<int> res;
		set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(res));
		return move(res);
	}
	int n;
	vector<ac_node> dat;
public:
	aho_corasick(const vector<string>& Ts) : n(Ts.size()), dat(1) {
		int now;
		dat[0].fail = 0;
		for (int i = 0; i < n; i++) {
			auto &T = Ts[i];
			now = 0;
			for (auto c : T) {
				if (dat[now].next[trans(c)] == -1) {
					dat[now].next[trans(c)] = dat.size();
					dat.emplace_back();
				}
				now = dat[now].next[trans(c)];
			}
			dat[now].ok.push_back(i);
		}
		queue<int> q;
		for (int i = 0; i < var; i++) {
			if (dat[0].next[i] == -1) {
				dat[0].next[i] = 0;
			}
			else {
				dat[dat[0].next[i]].fail = 0;
				q.push(dat[0].next[i]);
			}
		}
		while (!q.empty()) {
			now = q.front(); q.pop();
			for (int i = 0; i < var; i++) {
				if (dat[now].next[i] != -1) {
					int nx = dat[now].fail;
					while (dat[nx].next[i] == -1) {
						nx = dat[nx].fail;
					}
					int nex = dat[now].next[i];
					dat[nex].fail = dat[nx].next[i];
					dat[nex].ok = unite(dat[nex].ok, dat[dat[nx].next[i]].ok);
					q.push(nex);
				}
			}
		}
	}
	int size() const {
		return dat.size();
	}
	bool is_ok(int id) const {
		return !dat[id].ok.empty();
	}
	int get_next(int id, char c) const {
		while (dat[id].next[trans(c)] == -1) id = dat[id].fail;
		return dat[id].next[trans(c)];
	}
	vector<int> count(const string& S) const {
		vector<int> res(n);
		int now = 0;
		for (auto c : S) {
			now = get_next(now, c);
			for (auto k : dat[now].ok) res[k]++;
		}
		return res;
	}
	vector<vector<int>> all_count(const string& S) const {
		int N = S.size();
		vector<vector<int>> res(N);
		int now = 0;
		for (int i = 0; i < N; i++) {
			now = get_next(now, S[i]);
			res[i] = dat[now].ok;
		}
		return res;
	}
};
