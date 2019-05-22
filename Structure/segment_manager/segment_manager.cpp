
class segment_manager { // [l, r)
	map<int, int> pos;
	void add(int l, int r) {
		if (l == r) return;
		pos[l] = r;
	}
	void del(int l) {
		pos.erase(l);
	}
public:
	void add_range(int l, int r) {
		auto it = pos.lower_bound(l);
		if (it != pos.begin() && l <= prev(it)->second) --it, l = it->first;
		while (it != pos.end() && it->first <= r) {
			r = max(r, it->second);
			auto tmp = it++;
			del(tmp->first);
		}
		add(l, r);
	}
	void del_range(int l, int r) {
		auto it = pos.lower_bound(l);
		if (it != pos.begin() && l < prev(it)->second) {
			auto tmp = prev(it);
			int tl = tmp->first, tr = tmp->second;
			if (r <= tr) del(tl), add(tl, l), add(r, tr);
			else del(tl), add(tl, l);
		}
		while (it != pos.end() && it->first < r) {
			auto tmp = it; ++it;
			if (r < tmp->second) {
				int rr = tmp->second;
				del(tmp->first), add(r, rr);
			}
			else del(tmp->first);
		}
	}
	bool contain_all(int l, int r) {
		auto it = pos.upper_bound(l);
		return it != pos.begin() && r <= prev(it)->second;
	}
	bool contain_one(int l, int r) {
		auto it = pos.lower_bound(l);
		if (it != pos.end() && it->first < r) return true;
		if (it != pos.begin() && l < prev(it)->second) return true;
		return false;
	}
};
