#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // priority queue? greedy
    int n;
    long long t;
    cin >> n >> t;
    vector<long long> a(n), b(n);
    for (int i = 0; i < n; i++)
        cin >> a[i] >> b[i];
    
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> q; // cost, index?
    for (int i = 0; i < n; i++)
        q.push({a[i], i});

    long long res = 0;
    vector<int> vis(n, 0);
    while (t) {
        auto p = q.top();
        q.pop();
        if (t < p.first) break;
        int i = p.second;
        long long plays = 1;
        if (vis[i])
            plays = t / p.first;
        else 
            vis[i] = 1;

        res += plays;
        t -= (plays * p.first);

        q.push({a[p.second] + b[p.second], p.second});
    }
    
    cout << res << "\n";
    
    return 0;
}