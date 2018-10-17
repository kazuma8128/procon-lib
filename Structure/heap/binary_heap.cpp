
template <typename T>
class binary_heap {
	vector<T> data;
public:
	binary_heap() : data(1) {}
	int size() const {
		return data.size() - 1;
	}
	bool empty() const {
		return data.size() == 1;
	}
	T top() const {
		assert(data.size() > 1);
		return data[1];
	}
	void push(T x) {
		int p = data.size(), t;
		data.push_back(x);
		while ((t = p >> 1) > 0 && data[t] < x) { // min heap : >
			data[p] = data[t];
			p = t;
		}
		data[p] = x;
	}
	void pop() {
		assert(data.size() > 1);
		data[1] = data.back();
		data.pop_back();
		if (data.size() == 1) return;
		int p = 1;
		while (true) {
			int t = p, l = p << 1, r = l + 1;
			if (l < (int)data.size() && data[t] < data[l]) t = l; // min heap : >
			if (r < (int)data.size() && data[t] < data[r]) t = r; // min heap : >
			if (t == p) break;
			swap(data[p], data[t]);
			p = t;
		}
	}
};
