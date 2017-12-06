
// slow
class SuffixArray {
	const int n;
	string S;
	vector<int> sa, rank;
public:
	SuffixArray(const string &S_)
		: n(S_.size()), S(S_), sa(n + 1), rank(n + 1)
	{
		for (int i = 0; i <= n; i++) {
			sa[i] = i;
			rank[i] = i < n ? S[i] : -1;
		}

		vector<int> tmp(n + 1);
		for (int k = 1; k <= n; k *= 2) {
			auto Compare_SA = [=](int i, int j) {
				if (rank[i] != rank[j]) return rank[i] < rank[j];
				
				int ri = i + k <= n ? rank[i + k] : -1;
				int rj = j + k <= n ? rank[j + k] : -1;
				return ri < rj;
			};
			sort(sa.begin(), sa.end(), Compare_SA);

			tmp[sa[0]] = 0;
			for (int i = 1; i <= n; i++) {
				tmp[sa[i]] = tmp[sa[i - 1]] + (Compare_SA(sa[i - 1], sa[i]) ? 1 : 0);
			}
			for (int i = 0; i <= n; i++) {
				rank[i] = tmp[i];
			}
		}
	}
	bool Contain(const string &T) {
		int a = 0, b = n;
		while (b - a > 1) {
			int c = (a + b) / 2;
			if (S.compare(sa[c], T.length(), T) < 0) a = c;
			else b = c;
		}
		return S.compare(sa[b], T.length(), T) == 0;
	}
	vector<int> LCPArray() {
		for (int i = 0; i <= n; i++) rank[sa[i]] = i;

		int h = 0;
		vector<int> lcp(n + 1);
		for (int i = 0; i < n; i++) {
			int j = sa[rank[i] - 1];

			if (h > 0) h--;
			for (; j + h < n && i + h < n; h++) {
				if (S[j + h] != S[i + h]) break;
			}
			lcp[rank[i] - 1] = h;
		}
		return lcp;
	}
};
