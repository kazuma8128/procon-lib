
template <typename T>
vector<T> or_convolution(vector<T> a, vector<T> b) {
	int n = a.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				a[j | i] += a[j];
				b[j | i] += b[j];
			}
		}
	}
	for (int i = 0; i < n; i++) {
		a[i] *= b[i];
	}
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				a[j | i] -= a[j];
			}
		}
	}
	return a;
}

template <typename T>
vector<T> and_convolution(vector<T> a, vector<T> b) {
	int n = a.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				a[j] += a[j | i];
				b[j] += b[j | i];
			}
		}
	}
	for (int i = 0; i < n; i++) {
		a[i] *= b[i];
	}
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				a[j] -= a[j | i];
			}
		}
	}
	return a;
}

template <typename T>
vector<T> xor_convolution(vector<T> a, vector<T> b) {
	int n = a.size();
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = a[j], y = a[j | i];
				a[j] = x + y, a[j | i] = x - y;
			}
		}
	}
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = b[j], y = b[j | i];
				b[j] = x + y, b[j | i] = x - y;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		a[i] *= b[i];
	}
	T inv2 = T(1) / T(2);
	for (int i = 1; i < n; i <<= 1) {
		for (int j = 0; j < n; j++) {
			if ((j & i) == 0) {
				T x = a[j], y = a[j | i];
				a[j] = (x + y) * inv2, a[j | i] = (x - y) * inv2;
			}
		}
	}
	return a;
}
