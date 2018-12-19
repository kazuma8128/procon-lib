
bool is_convex(const vector<point>& ps) {
	const int n = ps.size();
	if (n < 3) return false;
	for (int i = 0; i < n; i++) {
		int c = ccw(ps[i], ps[(i + 1) % n], ps[(i + 2) % n]);
		if (c != 1) return false;
	}
	return true;
}

vector<point> convex_hull(vector<point> ps) {
	sort(ps.begin(), ps.end(), [](const point& a, const point& b) {
		return a.real() == b.real() ? a.imag() < b.imag() : a.real() < b.real();
	});
	int k = 0;
	const int n = ps.size();
	vector<point> qs(n + 1);
	for (int i = 0; i < n; ++i) {
		while (k > 1 && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) {
			k--;
		}
		qs[k++] = ps[i];
	}
	for (int i = n - 2, t = k; i >= 0; --i) {
		while (k > t && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) {
			k--;
		}
		qs[k++] = ps[i];
	}
	qs.resize(k - 1);
	return qs;
}

vector<point> convex_cut(const vector<point>& ps, line l) {
	const int n = ps.size();
	vector<point> res;
	for (int i = 0; i < n; ++i) {
		auto a = ps[i], b = ps[(i + 1) % n];
		if (ccw(l.a, l.b, a) != -1) {
			res.push_back(a);
		}
		if (ccw(l.a, l.b, a) * ccw(l.a, l.b, b) < 0) {
			if (cross(a - b, l.a - l.b) == 0) continue;
			res.push_back(is_ll(line(a, b), l));
		}
	}
	return res;
}

ld convex_diameter(const vector<point>& ps) {
	const int n = ps.size();
	ld d = 0;
	for (int i = 0, j = 0; i < n; ++i) {
		const point& p = ps[i];
		for (int k = (j + 1) % n; abs(ps[k] - p) > abs(ps[j] - p); k = (j + 1) % n) {
			j = k;
		}
		d = max(d, abs(ps[j] - p));
	}
	return d;
}
