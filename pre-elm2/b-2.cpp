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

    if (n == 1) {
        long long res = 0;
        if (t >= a[0]) {
            t -= a[0];
            res += 1LL + (t / (a[0] + b[0]));
        }
        cout << res << "\n";
        return 0;
    }
    
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> q; // cost, index?
    for (int i = 0; i < n; i++) {
        q.push({a[i], i});
    }

    auto p1 = q.top();
    q.pop();
    long long aa = p1.first;
    long long ab = p1.first + b[p1.second];
    auto p2 = q.top();
    long long ba = p2.first;
    long long bb = p2.first + b[p2.second];

    if (t < ab) {
        cout << "0\n";
        return 0;
    }

    t -= aa;
    long long res = 1;
    if (ab <= ba) {
        res += (t / (ab));
    } else {
        // switching with b
        // binary search
        long long l = 0;
        long long r = t / aa;
        long long mx = 0;
        while (l <= r) {
            long long m = (l + r) / 2;
            long long curr = m * (aa + ba);
            if (curr <= t) {
                mx = max(mx, m);
                l = m + 1;
            } else 
                r = m - 1;
        }

        res += mx;
        t -= (mx * (aa + ba));
        if (t >= ba)
            res++;
    }

    cout << res << "\n";
    
    return 0;
}