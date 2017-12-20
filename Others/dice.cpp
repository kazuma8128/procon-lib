
using pii = pair<int, int>;

// faces are numbered 0:top, 1:front, 2:right, 3:left, 4:back, 5:bottom
using pii = pair<int, int>;

struct dice {
	vector<pii> x;
	dice(int x_[6] = (int[]) { 1, 2, 3, 4, 5, 6 }) : x(3) {
		x[0] = pii(x_[0], x_[5]);
		x[1] = pii(x_[1], x_[4]);
		x[2] = pii(x_[2], x_[3]);
	}
	dice(const dice& d) : x(d.x) {}
	// copy dice
	dice& operator=(dice d) {
		x = d.x;
		return *this;
	}
	// get value on n-th face
	int X(int n) {
		return n < 3 ? x[n].first : x[5 - n].second;
	}
	// get value on side face to dir
	int Xside(int dir) {
		return dir / 2 ? x[2 - (dir & 1)].second : x[2 - (dir & 1)].first;
	}
	// roll to the north
	void N() {
		swap(x[0], x[1]);
		swap(x[1].first, x[1].second);
	}
	// roll to the east
	void E() {
		swap(x[0], x[2]);
		swap(x[2].first, x[2].second);
	}
	// roll to the west
	void W() {
		swap(x[0], x[2]);
		swap(x[0].first, x[0].second);
	}
	// roll to the south
	void S() {
		swap(x[0], x[1]);
		swap(x[0].first, x[0].second);
	}
	// roll
	void roll(int dir) {
		switch (dir)
		{
		case 0: W(); break;
		case 1: S(); break;
		case 2: E(); break;
		case 3: N(); break;
		default: break;
		}
	}
	// roll clockwise
	void C() {
		swap(x[1], x[2]);
		swap(x[2].first, x[2].second);
	}
	// roll counterclockwise
	void RC() {
		swap(x[1], x[2]);
		swap(x[1].first, x[1].second);
	}
	// roll so that val is on the top
	void top(int val) {
		if (x[0].first == val) {
			return;
		}
		if (x[1].first == val) {
			N();
		}
		else if (x[2].first == val) {
			E();
		}
		else if (x[2].second == val) {
			W();
		}
		else if (x[1].second == val) {
			S();
		}
		else {
			N(); N();
		}
	}
	// roll clockwise or counterclockwise so that val is on the front
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
};

bool operator==(const dice& l, const dice& r) {
	return l.x == r.x;
}

bool operator<(const dice& l, const dice& r) {
	return l.x < r.x;
}

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { 1, 0, -1, 0 };
