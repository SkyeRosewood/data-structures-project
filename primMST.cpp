#include "primMST.h"
#include <vector>
using namespace std;

static const double INF = 1e18;

MSTResult PrimMST::run(const Graph& graph) {
    MSTResult result;
    result.totalCost = 0.0;
    result.isConnected = graph.isConnected();
    if (!result.isConnected) return result;

    int V = graph.getVertexCount();
    vector<double> key(V, INF);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);
    key[0] = 0.0;

    MinHeap heap(V);
    heap.insert(HeapNode(0, 0.0, -1));

    while (!heap.isEmpty()) {
        HeapNode cur = heap.extractMin();
        int u = cur.index;
        if (inMST[u]) continue;
        inMST[u] = true;

        for (Edge* e = graph.getUndirAdj(u); e; e = e->next) {
            int v = e->dest;
            if (!inMST[v] && e->cost < key[v]) {
                key[v]=e->cost; parent[v]=u;
                if (heap.contains(v)) heap.decreaseKey(v, e->cost, u);
                else heap.insert(HeapNode(v, e->cost, u));
            }
        }
    }

    for (int i = 1; i < V; ++i) {
        if (parent[i] == -1) continue;
        MSTEdgeResult er;
        er.from = graph.codeOf(parent[i]);
        er.to = graph.codeOf(i);
        er.cost = key[i];
        result.edges.push_back(er);
        result.totalCost += key[i];
    }
    return result;
}