#include "kruskalMST.h"

UnionFind::UnionFind(int n) : n(n) {
    parent = new int[n];
    rank = new int[n];
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

UnionFind::~UnionFind() {
    delete[] parent;
    delete[] rank;
}

int UnionFind::find(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];
    while (parent[x] != root) {
        int next = parent[x];
        parent[x] = root;
        x = next;
    }
    return root;
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;

    if (rank[rx] < rank[ry]) { int t = rx; rx = ry; ry = t; }
    parent[ry] = rx;

    if (rank[rx] == rank[ry]) ++rank[rx];
    return true;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}

// ─── KruskalMST ──────────────────────────────────────────────────────────────

MSTResult KruskalMST::run(const Graph& graph) {
    return graph.kruskalMST();
}