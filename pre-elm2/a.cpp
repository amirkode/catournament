#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // flatten?
    int n, l;
    cin >> n >> l;
    vector<int> pref(n);
    int a;
    cin >> a;
    pref[0] = a;
    for (int i = 1; i < n; i++) {
        cin >> a;
        pref[i] = pref[i - 1] + a;
    }

    vector<int> d(pref[n - 1] + 1, 0);
    bool ok = false;
    int x, y;
    while (l--) {
        cin >> x >> y;
        if (ok) continue;
        int days = 0;
        if (y > 1)
            days = pref[y - 2];
        days += x;
        if (days > 2 && d[days - 2] && !d[days - 1]) {
            // kecepit
            ok = true;
        }
        d[days] = 1;
    }

    if (ok)
        cout << "YA\n";
    else
        cout << "TIDAK\n";
    
    return 0;
}