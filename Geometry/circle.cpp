
struct circle {
	point p;
	ld r;
	circle() {}
	circle(point p_, ld r_)
		: p(p_), r(r_) {}
};

int nct(circle c1, circle c2) {
	ld d = abs(c1.p - c2.p);
	ld r1 = max(c1.r, c2.r), r2 = min(c1.r, c2.r);
	if (d > r1 + r2 + eps) return 4;
	if (eq(d, r1 + r2)) return 3;
	if (d > r1 - r2 + eps) return 2;
	if (eq(d, r1 - r2)) return 1;
	return 0;
}

vector<point> is_cc(circle c1, circle c2) {
	vector<point> res;
	ld d = abs(c1.p - c2.p);
	assert(d != 0.0);
	ld rc = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
	ld dfr = c1.r * c1.r - rc * rc;
	if (abs(dfr) < eps) dfr = 0.0;
	if (dfr < 0.0) return res;
	ld rs = sqrt(dfr);
	point diff = (c2.p - c1.p) / d;
	res.push_back(c1.p + diff * point(rc, rs));
	res.push_back(c1.p + diff * point(rc, -rs));
	return res;
}

vector<point> is_lc(circle c, line l) {
	vector<point> res;
	ld d = dist_lp(l, c.p);
	if (d > c.r + eps) return res;
	ld len = (d > c.r) ? 0.0 : sqrt(c.r * c.r - d * d);
	point nor = (l.a - l.b) / abs(l.a - l.b);
	res.push_back(proj(l, c.p) + len * nor);
	res.push_back(proj(l, c.p) - len * nor);
	return res;
}

vector<point> is_sc(circle c, line s) {
	vector<point> v = is_lc(c, s), res;
	for (size_t k = 0; k < v.size(); k++) {
		if (ccw(s.a, v[k], s.b) == -2) {
			res.push_back(v[k]);
		}
	}
	return res;
}

vector<line> tangent_cp(circle c, point p) {
	vector<line> res;
	point v = c.p - p;
	ld d = abs(v);
	ld l = sqrt(norm(v) - c.r * c.r);
	if (isnan(l)) return res;
	point v1 = v * point(l / d, c.r / d);
	point v2 = v * point(l / d, -c.r / d);
	res.push_back((line) { p, p + v1 });
	if (l < eps) return res;
	res.push_back((line) { p, p + v2 });
	return res;
}

vector<line> tangent_cc(circle c1, circle c2) {
	vector<line> res;
	if (abs(c1.p - c2.p) - (c1.r + c2.r) > -eps) {
		point center = (c1.p * c2.r + c2.p * c1.r) / (c1.r + c2.r);
		res = tangent_cp(c1, center);
	}
	if (abs(c1.r - c2.r) > eps) {
		point out = (-c1.p * c2.r + c2.p * c1.r) / (c1.r - c2.r);
		vector<line> nres = tangent_cp(c1, out);
		res.insert(res.end(), nres.begin(), nres.end());
	}
	else {
		point v = c2.p - c1.p; v /= abs(v);
		point q1 = c1.p + v * point(0, 1) * c1.r;
		point q2 = c1.p + v * point(0, -1) * c1.r;
		res.push_back((line) { q1, q1 + v });
		res.push_back((line) { q2, q2 + v });
	}
	return res;
}
