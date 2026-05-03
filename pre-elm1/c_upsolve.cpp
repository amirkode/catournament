#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int mod = 998244353;
    int n, m;
    cin >> n >> m;
    vector<int> a(m);
    unordered_map<int, int> cnt;
    for (int i = 0; i < m; i++) {
        cin >> a[i];
        if (a[i])
            cnt[a[i]]++;
    }

    auto modpow = [&](long long a, long long b) -> long long {
        long long res = 1;
        while (b) {
            if (b & 1)
                res = (res * a) % mod;
            a = (a * a) % mod;
            b >>= 1;
        }

        return res;
    };
    
    vector<int> fact(n + 1);
    vector<int> invfact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++)
        fact[i] = (1LL * fact[i - 1] * i) % mod;
    invfact[n] = modpow(fact[n], mod - 2);
    for (int i = n - 1; i >= 0; i--)
        invfact[i] = (1LL * invfact[i + 1] * (i + 1)) % mod;
    
    int res = fact[n];
    for (int x: a)
        res = (1LL * res * invfact[x]) % mod;
    
    for (auto& [b, c]: cnt)
        res = (1LL * res * invfact[c]) % mod;

    cout << res << "\n";

    return 0;
}
