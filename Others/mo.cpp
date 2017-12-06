
const int N_MAX = 1e6;

const int B = sqrt(N_MAX);

struct query {
	int l, r, id;
	query(int l, int r, int id) : l(l), r(r), id(id) {}
	bool operator<(const query& q) const {
		return l / B != q.l / B ? l / B < q.l / B : r < q.r;
	}
};
