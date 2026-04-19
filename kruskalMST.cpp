#include "kruskalMST.h"
#include <vector>
using namespace std;

// ─── UnionFind ────────────────────────────────────────────────────────────────

UnionFind::UnionFind(int n) : n(n) {
    parent = new int[n]; rank = new int[n];
    for (int i=0; i<n; ++i) { parent[i]=i; rank[i]=0; }
}

UnionFind::~UnionFind() { delete[] parent; delete[] rank; }

int UnionFind::find(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];
    while (parent[x] != root) { int nx=parent[x]; parent[x]=root; x=nx; }
    return root;
}

bool UnionFind::unite(int x, int y) {
    int rx=find(x), ry=find(y);
    if (rx==ry) return false;
    if (rank[rx]<rank[ry]) { int t=rx; rx=ry; ry=t; }
    parent[ry]=rx;
    if (rank[rx]==rank[ry]) ++rank[rx];
    return true;
}

bool UnionFind::connected(int x, int y) { return find(x)==find(y); }

// ─── Merge sort for edges ─────────────────────────────────────────────────────

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
    vector<UndirectedEdge> sorted = edges;
    sortEdges(sorted);

    UnionFind uf(V);
    for (const UndirectedEdge& ue : sorted) {
        if (uf.unite(ue.u, ue.v)) {
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