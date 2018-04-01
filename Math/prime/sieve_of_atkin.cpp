
const int MAX = 1e7;
const int MAXSQ = ceil(sqrt(MAX));
bool is_p[MAX + 1];

void sieve_of_atkin() {
	int n;
	for (int z = 1; z <= 5; z += 4) {
		for (int y = z; y <= MAXSQ; y += 6) {
			for (int x = 1; x <= MAXSQ && (n = 4 * x*x + y*y) <= MAX; ++x)
				is_p[n] = !is_p[n];
			for (int x = y + 1; x <= MAXSQ && (n = 3 * x*x - y*y) <= MAX; x += 2)
				is_p[n] = !is_p[n];
		}
	}
	for (int z = 2; z <= 4; z += 2) {
		for (int y = z; y <= MAXSQ; y += 6) {
			for (int x = 1; x <= MAXSQ && (n = 3 * x*x + y*y) <= MAX; x += 2)
				is_p[n] = !is_p[n];
			for (int x = y + 1; x <= MAXSQ && (n = 3 * x*x - y*y) <= MAX; x += 2)
				is_p[n] = !is_p[n];
		}
	}
	for (int y = 3; y <= MAXSQ; y += 6) {
		for (int z = 1; z <= 2; ++z) {
			for (int x = z; x <= MAXSQ && (n = 4 * x*x + y*y) <= MAX; x += 3)
				is_p[n] = !is_p[n];
		}
	}
	for (int n = 5; n <= MAXSQ; ++n)
		if (is_p[n])
			for (int k = n*n; k <= MAX; k += n*n)
				is_p[k] = false;
	is_p[2] = is_p[3] = true;
}
