#pragma once
#include "template.hpp"

enum class Connectivity : u8 {
    Four = 4,
    Eight = 8,
};

template <typename V>
vvi grid_bfs(const vc<V>& g, const vc<pi>& s, V wall, Connectivity conn = Connectivity::Four) {
    const int n = len(g), m = len(g[0]);
    VECI(dis, n, m, inf<int>);
    std::deque<pi> q;
    foreach (sx, sy, s) {
        dis[sx][sy] = 0;
        q.eb(sx, sy);
    }
    constexpr int dx[]{0, 0, -1, 1, -1, -1, 1, 1};
    constexpr int dy[]{-1, 1, 0, 0, -1, 1, -1, 1};
    while (!q.empty()) {
        auto [x, y] = pop(q);
        _for (i, int(conn)) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || count(wall, g[nx][ny]) || dis[nx][ny] != inf<int>)
                continue;
            dis[nx][ny] = dis[x][y] + 1;
            q.eb(nx, ny);
        }
    }
    return dis;
}
template <typename V>
vvi grid_bfs(const vc<V>& g, int sx, int sy, V wall, Connectivity conn = Connectivity::Four) { return grid_bfs(g, {{sx, sy}}, wall, conn); }