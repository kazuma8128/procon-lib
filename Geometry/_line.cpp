
struct line {
	point a, b;
	line() {}
	line(const point& a_, const point &b_)
		: a(a_), b(b_) {}
};

bool isis_ll(line l, line m) {
	return std::abs(cross(l.b - l.a, m.b - m.a)) > eps;
}

bool isis_ls(line l, line s) {
	return (cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - l.a) < eps);
}

bool isis_lp(line l, point p) {
	return std::abs(cross(l.b - p, l.a - p)) < eps;
}

bool isis_sp(line s, point p) {
	return abs(s.a - p) + abs(s.b - p) - abs(s.b - s.a) < eps;
}

point proj(line l, point p) {
	ld t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
	return l.a + t * (l.a - l.b);
}

point is_ll(line s, line t) {
	point sv = s.b - s.a, tv = t.b - t.a;
	assert(cross(sv, tv) != 0);
	return s.a + sv * cross(tv, t.a - s.a) / cross(tv, sv);
}

bool isis_ss(line s, line t) {
	if (isis_ll(s, t)) return isis_ls(s, t) && isis_ls(t, s);
	return isis_sp(s, t.a) || isis_sp(s, t.b) || isis_sp(t, s.a);
}

ld dist_lp(line l, point p) {
	return abs(p - proj(l, p));
}

ld dist_ll(line l, line m) {
	return isis_ll(l, m) ? 0 : dist_lp(l, m.a);
}

ld dist_ls(line l, line s) {
	return isis_ls(l, s) ? 0 : min(dist_lp(l, s.a), dist_lp(l, s.b));
}

ld dist_sp(line s, point p) {
	point r = proj(s, p);
	return isis_sp(s, r) ? abs(r - p) : min(abs(s.a - p), abs(s.b - p));
}

ld dist_ss(line s, line t) {
	if (isis_ss(s, t)) return 0;
	return min({ dist_sp(s, t.a), dist_sp(s, t.b), dist_sp(t, s.a), dist_sp(t, s.b) });
}

istream& operator >> (istream& is, line& l) {
	point a, b; is >> a >> b;
	l = line(a, b);
	return is;
}

ostream& operator << (ostream& os, const line& l) {
	os << l.a << ' ' << l.b;
	return os;
}
