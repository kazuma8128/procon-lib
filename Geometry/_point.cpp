
using ld = long double;
const ld inf = 1e20;
const ld eps = 1e-10;
const ld pi = acos(-1.0);

using point = complex<ld>;

ld dot(point a, point b) {
	return real(conj(a) * b);
}

ld cross(point a, point b) {
	return imag(conj(a) * b);
}

int ccw(point a, point b, point c) {
	b -= a, c -= a;
	if (cross(b, c) > eps) return 1;	// a,b,c counter-clockwise
	if (cross(b, c) < -eps) return -1;	// a,b,c clockwise
	if (dot(b, c) < 0) return 2;		// c,a,b on a line
	if (norm(b) < norm(c)) return -2;	// a,b,c on a line
	return 0;							// a,c,b on a line
}

istream& operator >> (istream& is, point& p) {
	ld x, y; is >> x >> y;
	p = point(x, y);
	return is;
}

ostream& operator << (ostream& os, const point& p) {
	os << p.real() << ' ' << p.imag();
	return os;
}
