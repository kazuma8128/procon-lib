
using ll = long long;

ll digit_dp(ll n) {
	string S;
	while (n) S.push_back('0' + n % 10), n /= 10;
	reverse(S.begin(), S.end());
	n = S.size();
	vector<vector<vector<ll>>> dp(n + 1, vector<vector<ll>>(2, vector<ll>(10)));
	dp[0][0][0] = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 2; j++) {
			int lim = j ? 9 : S[i] - '0';
			for (int d = 0; d <= lim; d++) {
				for (int k = 0; k < 10; k++) {
					dp[i + 1][j || d < lim][k + (d == 1)] += dp[i][j][k];
				}
			}
		}
	}
	ll res = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			res += dp[n][i][j] * j;
		}
	}
	return res;
}
