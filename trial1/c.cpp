#include <bits/stdc++.h>

using namespace std;

int main() {
    /// hmm, simple counting?
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    string a, b;
    vector<int> cnta(26, 0);
    vector<int> cntb(26, 0);
    cin >> n;
    cin >> a;
    cin >> b;

    // cout << "a: " << a << "\n";
    // cout << "b: " << b << "\n";

    for (char c: a)
        cnta[c - 'a']++;
    for (char c: b)
        cntb[c - 'a']++;
    
    int i = 0, j = 0;
    string res = "";
    while (i < 26 && j < 26) {
        while (i < 26 && !cnta[i])
            i++;
        while (j < 26 && !cntb[j])
            j++;
        if (i == 26 || j == 26)
            break;

        res += char(i + 'a');
        res += char(j + 'a');
        cnta[i]--;
        cntb[j]--;
    }
    
    cout << res;
    
    return 0;
}
