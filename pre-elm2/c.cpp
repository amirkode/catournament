#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n);
    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        adj[u - 1].push_back({v - 1, w});
        adj[v - 1].push_back({u - 1, w});
    }
    
    // mst, prims
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
    vector<int> vis(n, 0);

    int res = 0;
    // init
    q.push({0, 0});
    while (!q.empty()) {
        auto t = q.top();
        q.pop();

        int w = t.first;
        int u = t.second;

        if (vis[u]) continue;

        res++; // node cnt
        vis[u] = 1;
        for (auto& next: adj[u]) {
            int v = next.first;
            int w = next.second;
            if (vis[v]) continue;
            q.push({w, v});
        }
    }
    
    return 0;
}