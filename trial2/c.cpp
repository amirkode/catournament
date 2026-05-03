#include<bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    string a, b;
    cin >> a >> b;
    
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    
    if (a > b)
        swap(a, b);
    
    string res = "";
    for (int i = 0; i < n; i++) {
        res += a[i];
        res += b[i];
    }

    cout << res << "\n";

    return 0;
}