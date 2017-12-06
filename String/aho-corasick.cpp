
const int var = 26;	// the number of charactor types
int trans(char c) {
	return c - 'a';
}

struct node {
	node *fail;
	vector<node*> next;
	vector<int> ok;
	node() : fail(nullptr), next(var, nullptr) {}
};

node* getnext(node* p, char c) {
	while (p->next[trans(c)] == nullptr) p = p->fail;
	return p->next[trans(c)];
}

class Aho_Corasick {
	vector<int> unite(const vector<int>& a, const vector<int>& b) {
		vector<int> res;
		set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(res));
		return res;
	}
	int K;
	node *root;
public:
	Aho_Corasick(const vector<string>& Ts) : K(Ts.size()), root(new node) {
		node *now;
		root->fail = root;
		for (int i = 0; i < K; i++) {
			auto &T = Ts[i];
			now = root;
			for (auto c : T) {
				if (now->next[trans(c)] == nullptr) {
					now->next[trans(c)] = new node;
				}
				now = now->next[trans(c)];
			}
			now->ok.push_back(i);
		}
		queue<node*> q;
		for (int i = 0; i < var; i++) {
			if (root->next[i] == nullptr) {
				root->next[i] = root;
			}
			else {
				root->next[i]->fail = root;
				q.push(root->next[i]);
			}
		}
		while (!q.empty()) {
			now = q.front(); q.pop();
			for (int i = 0; i < var; i++) {
				if (now->next[i] != nullptr) {
					node *nx = now->fail;
					while (nx->next[i] == nullptr) {
						nx = nx->fail;
					}
					now->next[i]->fail = nx->next[i];
					now->next[i]->ok = unite(now->next[i]->ok, nx->next[i]->ok);
					q.push(now->next[i]);
				}
			}
		}
	}
	node* getroot() const {
		return root;
	}
	vector<int> count(const string& S) const {
		vector<int> res(K);
		node *now = root;
		for (auto c : S) {
			now = getnext(now, c);
			for (auto k : now->ok) res[k]++;
		}
		return res;
	}
};
