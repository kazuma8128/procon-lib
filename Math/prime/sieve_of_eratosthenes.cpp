
vector<bool> sieve_of_eratosthenes(int size) {
	vector<bool> res(size + 1, 1);
	res[0] = res[1] = false;
	for (int i = 2; i * i <= size; i++) if (res[i]) {
		for (int j = i * 2; j <= size; j += i) res[j] = false;
	}
	return res;
}
