
ld area(const vector<point>& ps) {
	const int n = ps.size();
	ld a = 0;
	for (int i = 0; i < n; i++) {
		a += cross(ps[i], ps[(i + 1) % n]);
	}
	return abs(a / 2);
}

// 0:out, 1:on, 2:in
int is_in_polygon(const vector<point>& poly, point p) {
	const int n = poly.size();
	ld sum = 0;
	for (int i = 0; i < n; ++i) {
		point p1 = poly[i], p2 = poly[(i + 1) % n];
		if (isis_sp(line(p1, p2), p)) return 1;
		sum += arg((p2 - p) / (p1 - p));
	}
	return std::abs(sum) < pi ? 0 : 2;
}
