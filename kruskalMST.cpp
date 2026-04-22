#include "kruskalMST.h"
#include <vector>
using namespace std;

// ─── UnionFind ────────────────────────────────────────────────────────────────

UnionFind::UnionFind(int n) : n(n) {
    parent = new int[n]; 
    rank = new int[n];
    
    // Initially, every vertex is its own independent tree (parent is itself)
    // Rank (approximate depth) starts at 0 for all.
    for (int i=0; i<n; ++i) { 
        parent[i] = i; 
        rank[i] = 0; 
    }
}

UnionFind::~UnionFind() { delete[] parent; delete[] rank; }

int UnionFind::find(int x) {
    // Phase 1: Traverse up the tree to find the ultimate root ancestor
    int root = x;
    while (parent[root] != root) root = parent[root];
    
    // Phase 2: PATH COMPRESSION
    // Traverse the path again, attaching every visited node directly to the root.
    // This flattens the tree so future lookups are O(1).
    while (parent[x] != root) { 
        int nx = parent[x]; 
        parent[x] = root; 
        x = nx; 
    }
    return root;
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    
    // If they share the same root, they are already connected. 
    // Adding this edge would create a cycle, so we reject it.
    if (rx == ry) return false;
    
    // Ensure rx represents the taller tree to keep the combined tree shallow.
    if (rank[rx] < rank[ry]) { 
        int t = rx; rx = ry; ry = t; 
    }
    
    // Attach the shorter tree (ry) under the root of the taller tree (rx)
    parent[ry] = rx;
    
    // If they were the exact same height, the new combined tree is now one level deeper.
    if (rank[rx] == rank[ry]) ++rank[rx];
    
    return true;
}

bool UnionFind::connected(int x, int y) { return find(x) == find(y); }

// ─── Merge sort for edges ─────────────────────────────────────────────────────

// Kruskal's is a greedy algorithm, so it must evaluate the cheapest edges first.
// This is a standard O(E log E) Merge Sort to order edges by cost.
static void sortEdges(vector<UndirectedEdge>& arr) {
    int n = (int)arr.size();
    if (n <= 1) return;
    int mid = n / 2;
    vector<UndirectedEdge> L(arr.begin(), arr.begin()+mid);
    vector<UndirectedEdge> R(arr.begin()+mid, arr.end());
    sortEdges(L); sortEdges(R);
    int i=0, j=0, k=0;
    while (i<(int)L.size() && j<(int)R.size())
        arr[k++] = (L[i].cost <= R[j].cost) ? L[i++] : R[j++];
    while (i<(int)L.size()) arr[k++]=L[i++];
    while (j<(int)R.size()) arr[k++]=R[j++];
}

// ─── KruskalMST ───────────────────────────────────────────────────────────────

MSTResult KruskalMST::run(const Graph& graph) {
    MSTResult result;
    result.totalCost = 0.0;
    result.isConnected = graph.isConnected();

    const vector<UndirectedEdge>& edges = graph.getUndirEdges();
    if (edges.empty()) return result;

    int V = graph.getVertexCount();
    
    // Step 1: Copy and sort all edges from cheapest to most expensive
    vector<UndirectedEdge> sorted = edges;
    sortEdges(sorted);

    // Step 2: Initialize the disjoint-set for cycle detection
    UnionFind uf(V);
    
    // Step 3: Greedily evaluate each edge
    for (const UndirectedEdge& ue : sorted) {
        
        // uf.unite() does two things at once here:
        // 1. Checks if adding the edge creates a cycle.
        // 2. If it doesn't create a cycle, it merges their trees.
        // It returns true ONLY if the vertices were in separate trees.
        if (uf.unite(ue.u, ue.v)) {
            
            // The edge is safe! Add it to our final Minimum Spanning Tree.
            MSTEdgeResult er;
            er.from = graph.codeOf(ue.u);
            er.to = graph.codeOf(ue.v);
            er.cost = ue.cost;
            
            result.edges.push_back(er);
            result.totalCost += ue.cost;
        }
    }
    
    return result;
}