
using pii = pair<int, int>;

struct dice {
	vector<pii> x;
	dice(int x_[6] = (int[]) { 1, 2, 3, 4, 5, 6 }) : x(3) {
		x[0] = pii(x_[0], x_[5]);
		x[1] = pii(x_[1], x_[4]);
		x[2] = pii(x_[2], x_[3]);
	}
	dice(const dice& d) : x(d.x) {}
	dice& operator=(dice d) {
		x = d.x;
		return *this;
	}
	int X(int n) {
		return n < 3 ? x[n].first : x[5 - n].second;
	}
	void N() {
		swap(x[0], x[1]);
		swap(x[1].first, x[1].second);
	}
	void E() {
		swap(x[0], x[2]);
		swap(x[0].first, x[0].second);
	}
	void W() {
		swap(x[0], x[2]);
		swap(x[2].first, x[2].second);
	}
	void S() {
		swap(x[0], x[1]);
		swap(x[0].first, x[0].second);
	}
	void C() {
		swap(x[1], x[2]);
		swap(x[2].first, x[2].second);
	}
	void RC() {
		swap(x[1], x[2]);
		swap(x[1].first, x[1].second);
	}
	void top(int val) {
		if (x[0].first == val) {
			return;
		}
		if (x[1].first == val) {
			N();
		}
		else if (x[2].first == val) {
			W();
		}
		else if (x[2].second == val) {
			E();
		}
		else if (x[1].second == val) {
			S();
		}
		else {
			N(); N();
		}
	}
	void front(int val) {
		if (x[1].first == val) {
			return;
		}
		if (x[2].first == val) {
			C();
		}
		else if (x[2].second == val) {
			RC();
		}
		else {
			swap(x[1].first, x[1].second);
			swap(x[2].first, x[2].second);
		}
	}
	set<dice> all() {
		set<dice> res;
		dice d(*this);
		dice tmp(d);
		for (int i = 0; i < 4; i++) {
			tmp = d;
			for (int j = 0; j < 4; j++) {
				res.insert(tmp);
				tmp.C();
			}
			d.N();
		}
		d.W();
		tmp = d;
		for (int i = 0; i < 4; i++) {
			res.insert(tmp);
			tmp.C();
		}
		d.E(); d.E();
		tmp = d;
		for (int i = 0; i < 4; i++) {
			res.insert(tmp);
			tmp.C();
		}
		return res;
	}
};

bool operator==(const dice& l, const dice& r) {
	return l.x == r.x;
}

bool operator<(const dice& l, const dice& r) {
	return l.x < r.x;
}
