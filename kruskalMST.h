#ifndef KRUSKALMST_H
#define KRUSKALMST_H
#include "graph.h"

// ─── UnionFind ────────────────────────────────────────────────────────────────
class UnionFind {
public:
    explicit UnionFind(int n);
    ~UnionFind();

    int find(int x);
    bool unite(int x, int y);
    bool connected(int x, int y);

private:
    int* parent;
    int* rank;
    int n;
};

// ─── KruskalMST ───────────────────────────────────────────────────────────────
class KruskalMST {
public:
    static MSTResult run(const Graph& graph);
};

#endif