#include<bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, a;
    long long sum = 0;
    int mx = 0;
    cin >> n;
    while (n--) {
        cin >> a;
        sum += a;
        mx = max(mx, a);
    }

    int res = mx + 1;
    if (1LL * res == sum)
        res++;

    cout << res << "\n";

    return 0;
}