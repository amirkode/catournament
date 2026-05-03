#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // priority queue? greedy
    int n;
    long long t;
    cin >> n >> t;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++)
        cin >> a[i] >> b[i];
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q; // cost, index?
    for (int i = 0; i < n; i++)
        q.push({a[i], i});

    int res = 0;
    while (t) {
        auto p = q.top();
        q.pop();
        if (t < p.first) break;
        t -= p.first;
        res++;
        q.push({a[p.second] + b[p.second], p.second});
    }
    
    cout << res << "\n";
    
    return 0;
}