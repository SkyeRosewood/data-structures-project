#include "primMST.h"
#include <vector>
using namespace std;

static const double INF = 1e18;

MSTResult PrimMST::run(const Graph& graph) {
    MSTResult result;
    result.totalCost = 0.0;
    result.isConnected = graph.isConnected();

    int V = graph.getVertexCount();
    
    // Initialize tracking arrays for Prim's algorithm
    vector<double> key(V, INF);    // Minimum cost to reach each node
    vector<int> parent(V, -1);     // To reconstruct the tree later
    vector<bool> inMST(V, false);  // Tracks nodes already included

    // Outer loop ensures we process each disconnected graphs (spanning forests)
    for (int start = 0; start < V; ++start) {
        if (inMST[start]) continue;

        key[start] = 0.0;
        MinHeap heap(V);
        heap.insert(HeapNode(start, 0.0, -1));

        // Greedily grow the minimum spanning tree
        while (!heap.isEmpty()) {
            HeapNode cur = heap.extractMin();
            int u = cur.index;
            
            if (inMST[u]) continue;
            inMST[u] = true;

            // Evaluate adjacent edges to find cheaper connections to unvisited nodes
            for (Edge* e = graph.getUndirAdj(u); e; e = e->next) {
                int v = e->dest;
                
                if (!inMST[v] && e->cost < key[v]) {
                    key[v] = e->cost;
                    parent[v] = u;
                    
                    // Update the priority queue with the new shortest distance
                    if (heap.contains(v)) heap.decreaseKey(v, e->cost, u);
                    else heap.insert(HeapNode(v, e->cost, u));
                }
            }
        }
    }

    // Reconstruct the final MST edges from the parent array
    for (int i = 0; i < V; ++i) {
        if (parent[i] == -1) continue; // Skip root nodes (they have no parent edge)
        
        MSTEdgeResult er;
        er.from = graph.codeOf(parent[i]);
        er.to = graph.codeOf(i);
        er.cost = key[i];
        
        result.edges.push_back(er);
        result.totalCost += key[i];
    }
    
    return result;
}
