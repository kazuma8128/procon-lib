
const ld inf = 1e20;

ld closest_pair(vector<point> a) {
	sort(a.begin(), a.end(), [](const point& p, const point& q) {
		return p.real() == q.real() ? p.imag() < q.imag() : p.real() < q.real();
	});
	using pit = vector<point>::iterator;
	function<ld(pit, pit)> calc = [&](pit b, pit e) {
		if (e - b <= 1) return inf;
		pit m = b + ((e - b) >> 1);
		ld c = m->real();
		ld d = min(calc(b, m), calc(m, e));
		inplace_merge(b, m, e, [](const point& x, const point& y) {
			return x.imag() < y.imag();
		});
		vector<point> v;
		for (pit it = b; it != e; ++it) {
			if (std::abs(it->real() - c) >= d) continue;
			for (int j = v.size() - 1; j >= 0; --j) {
				ld dx = it->real() - v[j].real();
				ld dy = it->imag() - v[j].imag();
				if (dy >= d) break;
				d = min(d, sqrtl(dx * dx + dy * dy));
			}
			v.push_back(*it);
		}
		return d;
	};
	return calc(a.begin(), a.end());
}
