
void compress(vector<ll>& v, vector<ll>& vs, int& size) {
	vs = v;
	sort(vs.begin(), vs.end());
	vs.erase(unique(vs.begin(), vs.end()), vs.end());
	for (auto& val : v) {
		val = lower_bound(vs.begin(), vs.end(), val) - vs.begin();
	}
	size = vs.size();
}
