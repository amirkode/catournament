#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // stack?

    int n;
    vector<long long> st;
    long long a;
    cin >> n;
    while (n--) {
        cin >> a;
        st.push_back(a);
        while (st.size() > 1 && st[st.size() - 1] == st[st.size() - 2]) {
            st[st.size() - 2] *= 2;
            st.pop_back();
        }
        //cout << "st size: " << st.size() << "\n";
    }


    if (st.size() > 1)
        cout << "TIDAK\n";
    else
        cout << "YA\n";

    return 0;
}
