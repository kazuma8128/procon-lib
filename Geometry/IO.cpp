
istream& operator >> (istream& is, point& p) {
	ld x, y; is >> x >> y;
	p = point(x, y);
	return is;
}

istream& operator >> (istream& is, line& l) {
	point a, b; is >> a >> b;
	l = line(a, b);
	return is;
}

ostream& operator << (ostream& os, const point& p) {
	os << p.real() << ' ' << p.imag();
	return os;
}

ostream& operator << (ostream& os, const line& l) {
	os << l.a << ' ' << l.b;
	return os;
}
