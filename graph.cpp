#include "graph.h"
#include "minHeap.h"
#include "queue.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

static const double INF = 1e18;

// ─── Airport ──────────────────────────────────────────────────────────────────

Airport::Airport() : code(""), city("") {}
Airport::Airport(const string& code, const string& city) : code(code), city(city) {}

// ─── Edge ─────────────────────────────────────────────────────────────────────

Edge::Edge(int dest, double distance, double cost, Edge* next)
    : dest(dest), distance(distance), cost(cost), next(next) {}

// ─── UndirectedEdge ───────────────────────────────────────────────────────────

UndirectedEdge::UndirectedEdge() : u(0), v(0), cost(0.0) {}
UndirectedEdge::UndirectedEdge(int u, int v, double cost) : u(u), v(v), cost(cost) {}

// ─── PathResult ───────────────────────────────────────────────────────────────

PathResult::PathResult() : distance(0.0), cost(0.0), found(false) {}

// ─── Graph ────────────────────────────────────────────────────────────────────

Graph::Graph() : V(0), E(0) {}

Graph::~Graph() {
    for (int i = 0; i < V; ++i) {
        for (Edge* e = adjList[i]; e;) { Edge* nx = e->next; delete e; e = nx; }
        for (Edge* e = undirAdjList[i]; e;) { Edge* nx = e->next; delete e; e = nx; }
    }
}

// ─── Insert (called by csvReader) ─────────────────────────────────────────────

void Graph::addAirportAndEdge(const string& oCode, const string& oCity,
                               const string& dCode, const string& dCity,
                               double dist, double cost)
{
    int src = addOrGetAirport(oCode, oCity);
    int dest = addOrGetAirport(dCode, dCity);
    addEdge(src, dest, dist, cost);
}

int Graph::addOrGetAirport(const string& code, const string& city) {
    for (int i = 0; i < V; ++i) if (airports[i].code == code) return i;
    airports.push_back(Airport(code, city));
    adjList.push_back(nullptr);
    undirAdjList.push_back(nullptr);
    return V++;
}

void Graph::addEdge(int src, int dest, double distance, double cost) {
    adjList[src] = new Edge(dest, distance, cost, adjList[src]);
    ++E;
}

// ─── Getters ──────────────────────────────────────────────────────────────────

int    Graph::getVertexCount() const { return V; }
int    Graph::getEdgeCount() const { return E; }
string Graph::codeOf(int i) const { return (i>=0&&i<V)?airports[i].code:""; }
string Graph::cityOf(int i) const { return (i>=0&&i<V)?airports[i].city:""; }
Edge*  Graph::getAdjHead(int u) const { return (u>=0&&u<V)?adjList[u]:nullptr; }
Edge*  Graph::getUndirAdj(int u) const { return (u>=0&&u<V)?undirAdjList[u]:nullptr; }
bool   Graph::isConnected() const { return isUndirectedConnected(); }
const vector<UndirectedEdge>& Graph::getUndirEdges() const { return undirEdges; }

int Graph::indexOf(const string& code) const {
    for (int i = 0; i < V; ++i) if (airports[i].code == code) return i;
    return -1;
}

string Graph::extractState(const string& city) {
    size_t comma = city.rfind(',');
    if (comma == string::npos) return "";
    string st = city.substr(comma + 1);
    size_t s = st.find_first_not_of(" \t");
    return (s == string::npos) ? "" : st.substr(s);
}

// ─── Dijkstra (internal) ──────────────────────────────────────────────────────

void Graph::dijkstra(int src,
                     vector<double>& dist,
                     vector<double>& costArr,
                     vector<int>& parent) const
{
    dist.assign(V, INF); costArr.assign(V, INF); parent.assign(V, -1);
    dist[src] = costArr[src] = 0.0;

    MinHeap heap(V);
    heap.insert(HeapNode(src, 0.0, -1));

    while (!heap.isEmpty()) {
        HeapNode cur = heap.extractMin();
        int u = cur.index;
        for (Edge* e = adjList[u]; e; e = e->next) {
            int v = e->dest;
            double nd = dist[u] + e->distance;
            double nc = costArr[u] + e->cost;
            if (nd < dist[v]) {
                dist[v]=nd; costArr[v]=nc; parent[v]=u;
                if (heap.contains(v)) heap.decreaseKey(v, nd, u);
                else heap.insert(HeapNode(v, nd, u));
            }
        }
    }
}

PathResult Graph::buildPath(int dest,
                             const vector<double>& dist,
                             const vector<double>& costArr,
                             const vector<int>& parent) const
{
    PathResult res;
    res.destCode = airports[dest].code;
    if (dist[dest] >= INF) { res.found = false; return res; }
    res.found=true; res.distance=dist[dest]; res.cost=costArr[dest];
    vector<string> rev;
    for (int c = dest; c != -1; c = parent[c]) rev.push_back(airports[c].code);
    for (int i = (int)rev.size()-1; i >= 0; --i) res.path.push_back(rev[i]);
    return res;
}

// ─── Task 2 ───────────────────────────────────────────────────────────────────

PathResult Graph::shortestPath(const string& origin, const string& dest) const {
    PathResult res;
    int s = indexOf(origin), d = indexOf(dest);
    if (s==-1) { cerr<<"Airport not found: "<<origin<<"\n"; return res; }
    if (d==-1) { cerr<<"Airport not found: "<<dest<<"\n"; return res; }
    vector<double> dist, cost; vector<int> parent;
    dijkstra(s, dist, cost, parent);
    return buildPath(d, dist, cost, parent);
}

// ─── Task 3 ───────────────────────────────────────────────────────────────────

vector<PathResult> Graph::shortestPathsToState(const string& origin,
                                                const string& state) const
{
    vector<PathResult> results;
    int src = indexOf(origin);
    if (src==-1) { cerr<<"Airport not found: "<<origin<<"\n"; return results; }
    vector<double> dist, cost; vector<int> parent;
    dijkstra(src, dist, cost, parent);
    for (int i = 0; i < V; ++i)
        if (i!=src && extractState(airports[i].city)==state)
            results.push_back(buildPath(i, dist, cost, parent));
    return results;
}

// ─── Task 4 ───────────────────────────────────────────────────────────────────

PathResult Graph::shortestPathWithStops(const string& origin,
                                         const string& destination,
                                         int stops) const
{
    PathResult res;
    int src = indexOf(origin), dest = indexOf(destination);
    if (src==-1 || dest==-1) return res;

    int maxEdges = stops + 1;
    int sz = V * (maxEdges + 1);
    vector<double> dp(sz, INF);
    vector<int> par(sz, -1);
    dp[src * (maxEdges+1)] = 0.0;

    for (int e = 0; e < maxEdges; ++e)
        for (int u = 0; u < V; ++u) {
            double cur = dp[u*(maxEdges+1)+e];
            if (cur >= INF) continue;
            for (Edge* edge = adjList[u]; edge; edge = edge->next) {
                int v = edge->dest;
                double nd = cur + edge->distance;
                int idx = v*(maxEdges+1)+(e+1);
                if (nd < dp[idx]) { dp[idx]=nd; par[idx]=u; }
            }
        }

    int fi = dest*(maxEdges+1)+maxEdges;
    if (dp[fi] >= INF) { res.found=false; return res; }

    res.found=true; res.distance=dp[fi];
    vector<string> rev;
    int c=dest, e=maxEdges;
    while (c != -1) { rev.push_back(airports[c].code); int p=par[c*(maxEdges+1)+e--]; c=p; }
    for (int i=(int)rev.size()-1; i>=0; --i) res.path.push_back(rev[i]);

    res.cost = 0.0;
    for (int i = 0; i+1 < (int)res.path.size(); ++i) {
        int u=indexOf(res.path[i]), v=indexOf(res.path[i+1]);
        for (Edge* edge=adjList[u]; edge; edge=edge->next)
            if (edge->dest==v) { res.cost+=edge->cost; break; }
    }
    return res;
}

// ─── Task 5 ───────────────────────────────────────────────────────────────────

void Graph::sortConnections(vector<ConnectionInfo>& arr) {
    for (int i = 1; i < (int)arr.size(); ++i) {
        ConnectionInfo key = arr[i]; int j = i-1;
        while (j>=0 && arr[j].total < key.total) { arr[j+1]=arr[j]; --j; }
        arr[j+1] = key;
    }
}

vector<Graph::ConnectionInfo> Graph::getConnectionCounts() const {
    vector<int> in(V,0), out(V,0);
    for (int u=0; u<V; ++u)
        for (Edge* e=adjList[u]; e; e=e->next) { out[u]++; in[e->dest]++; }
    vector<ConnectionInfo> res(V);
    for (int i=0; i<V; ++i) {
        res[i].code=airports[i].code; res[i].inbound=in[i];
        res[i].outbound=out[i]; res[i].total=in[i]+out[i];
    }
    sortConnections(res);
    return res;
}

// ─── Task 6: Build G_u ────────────────────────────────────────────────────────

void Graph::buildUndirectedGraph() {
    for (int i=0; i<V; ++i) {
        for (Edge* e=undirAdjList[i]; e;) { Edge* nx=e->next; delete e; e=nx; }
        undirAdjList[i] = nullptr;
    }
    undirEdges.clear();

    vector<double> best(V*V, INF);
    vector<bool> has(V*V, false);
    for (int u=0; u<V; ++u)
        for (Edge* e=adjList[u]; e; e=e->next) {
            int idx=u*V+e->dest;
            if (e->cost < best[idx]) best[idx]=e->cost;
            has[idx]=true;
        }

    for (int u=0; u<V; ++u)
        for (int v=u+1; v<V; ++v) {
            bool uve=has[u*V+v], vue=has[v*V+u];
            if (!uve && !vue) continue;
            double cu=best[u*V+v], cv=best[v*V+u];
            double chosen = (uve&&vue) ? (cu<cv?cu:cv) : (uve?cu:cv);
            undirEdges.push_back(UndirectedEdge(u,v,chosen));
            undirAdjList[u] = new Edge(v, 0.0, chosen, undirAdjList[u]);
            undirAdjList[v] = new Edge(u, 0.0, chosen, undirAdjList[v]);
        }
}

bool Graph::isUndirectedConnected() const {
    if (V==0) return true;
    vector<bool> visited(V, false);
    Queue q; q.enqueue(0); visited[0]=true;
    while (!q.isEmpty()) {
        int u = q.dequeue();
        for (Edge* e=undirAdjList[u]; e; e=e->next)
            if (!visited[e->dest]) { visited[e->dest]=true; q.enqueue(e->dest); }
    }
    for (int i=0; i<V; ++i) if (!visited[i]) return false;
    return true;
}