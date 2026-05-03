#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n = 10, x;

    int found = 0;
    auto find = [&](int i, int j) -> void {
        int dir = -1; // 0 -> ver, 1 -> hor
        int up, down, left, right;
        // check vertical
        if (i > 0) {
            cout << i << " " << j + 1 << "\n";
            cout.flush();
            cin >> x;
            if (x == 1) {
                found++;
                up = i - 2;
                down = i + 1;
                dir = 0;
            }
        }
        if (dir == -1 && i < n - 1) {
            cout << i + 2 << " " << j + 1 << "\n";
            cout.flush();
            cin >> x;
            if (x == 1) {
                found++;
                up = -1;
                down = i + 2;
                dir = 0;
            }
        }
        // check horizontal
        if (dir == -1 && j > 0) {
            cout << i + 1 << " " << j << "\n";
            cout.flush();
            cin >> x;
            if (x == 1) {
                found++;
                left = j - 2;
                right = j + 1;
                dir = 1;
            }
        }
        if (dir == -1 && j < n - 1) {
            cout << i + 1 << " " << j + 2 << "\n";
            cout.flush();
            cin >> x;
            if (x == 1) {
                found++;
                left = -1;
                right = j + 2;
            }
        }

        if (dir == 0) {
            while (up >= 0) {
                cout << 1 + up-- << " " << j + 1 << "\n";
                cout.flush();
                cin >> x;
                if (x == 0 || x == -1) break;
                found++;
                if (found == 5) return;
            }
            while (down < n) {
                cout << 1 + down++ << " " << j + 1 << "\n";
                cout.flush();
                cin >> x;
                if (x == 0 || x == -1) break;
                found++;
                if (found == 5) return;
            }
        } else {
            while (left >= 0) {
                cout << i + 1 << " " << 1 + left-- << "\n";
                cout.flush();
                cin >> x;
                if (x == 0 || x == -1) break;
                found++;
                if (found == 5) return;
            }
            while (right < n) {
                cout << i + 1 << " " << 1 + right++ << "\n";
                cout.flush();
                cin >> x;
                if (x == 0 || x == -1) break;
                found++;
                if (found == 5) return;
            }
        }
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c = (i + j) % 5;
            if (!c) {
                cout << i + 1 << " " << j + 1 << "\n";
                cout.flush();
                cin >> x;
                if (x == -1) return 0;
                if (x == 0) continue;
                // found
                found++;
                find(i, j);
                return 0;
            }
        }
        //cout << endl;
    }
    return 0;
}