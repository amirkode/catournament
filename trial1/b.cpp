#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, x;
    cin >> n;
    long long sum = 0;
    while (n--) {
        cin >> x;
        sum += x;
    }

    cout << sum + 1 << "\n"; // must be valid
    
    return 0;
}
