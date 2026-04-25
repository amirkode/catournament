#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    string s, t;
    cin >> n;
    cin >> s;
    cin >> t;
    int dist;
    for (int i = 0; i < n; i++) {
        int currdist;
        if (s[i] > t[i]) {
            currdist = 10 - s[i] + t[i];
        } else {
            currdist = t[i] - s[i];
        }
        // cout << "currdist: " << currdist << endl;
        // cout << "dist: " << dist << endl;
        if (i == 0) {
            dist = currdist;
        } else if (dist != currdist) {
            cout << "TIDAK\n";
            return 0;
        }
    }

    cout << "YA\n";
    return 0;
}
