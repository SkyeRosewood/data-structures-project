#ifndef KRUSKALMST_H
#define KRUSKALMST_H
#include "graph.h"

//Tracks connected components so Kruskal's can safely add edges without forming cycles.
class UnionFind {
public:
    explicit UnionFind(int n);
    ~UnionFind();

    // 2. find the original root than attaches each node of the path to the root
    int find(int x);
    bool unite(int x, int y);
    bool connected(int x, int y);

private:
    int* parent;
    
    // 3. Uses "union-by-rank" to always attach shorter trees under taller ones, preventing deep, slow-to-search branches.
    int* rank;
    int n;
};

class KruskalMST {
public:
    static MSTResult run(const Graph& graph);
};

#endif