#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>

// ─── Airport ──────────────────────────────────────────────────────────────────

struct Airport {
    std::string code;
    std::string city;

    Airport();
    Airport(const std::string& code, const std::string& city);
};

// ─── Edge (directed, adjacency list node) ─────────────────────────────────────

struct Edge {
    int dest;
    double distance;
    double cost;
    Edge* next;

    Edge(int dest, double distance, double cost, Edge* next = nullptr);
};

// ─── UndirectedEdge (flat, used by Kruskal) ───────────────────────────────────

struct UndirectedEdge {
    int u, v;
    double cost;

    UndirectedEdge();
    UndirectedEdge(int u, int v, double cost);
};

// ─── PathResult ───────────────────────────────────────────────────────────────

struct PathResult {
    std::vector<std::string> path;
    std::string destCode;
    double distance;
    double cost;
    bool found;

    PathResult();
};

// ─── MSTResult ────────────────────────────────────────────────────────────────

struct MSTEdgeResult {
    std::string from;
    std::string to;
    double cost;
};

struct MSTResult {
    std::vector<MSTEdgeResult> edges;
    double totalCost;
    bool isConnected;
};

// ─── Graph ────────────────────────────────────────────────────────────────────

class Graph {
public:
    Graph();
    ~Graph();

    // Called by csvReader to insert one CSV row
    void addAirportAndEdge(const std::string& originCode,
                           const std::string& originCity,
                           const std::string& destCode,
                           const std::string& destCity,
                           double distance, double cost);

    // General info
    int getVertexCount() const;
    int getEdgeCount() const;
    int indexOf(const std::string& code) const;
    std::string codeOf(int index) const;
    std::string cityOf(int index) const;

    // Task 2: shortest path between two airports (Dijkstra on distance)
    PathResult shortestPath(const std::string& origin,
                            const std::string& destination) const;

    // Task 3: shortest paths from one airport to all airports in a state
    std::vector<PathResult> shortestPathsToState(const std::string& origin,
                                                  const std::string& stateAbbr) const;

    // Task 4: shortest path with exact number of intermediate stops
    PathResult shortestPathWithStops(const std::string& origin,
                                     const std::string& destination,
                                     int stops) const;

    // Task 5: connection counts sorted descending by total
    struct ConnectionInfo {
        std::string code;
        int inbound, outbound, total;
    };
    std::vector<ConnectionInfo> getConnectionCounts() const;

    // Task 6: build undirected graph G_u from directed graph G
    void buildUndirectedGraph();

    // ── Accessors used by DFS, BFS, PrimMST, KruskalMST ─────────────────────
    Edge* getAdjHead(int u) const;
    Edge* getUndirAdj(int u) const;
    bool  isConnected() const;
    const std::vector<UndirectedEdge>& getUndirEdges() const;

private:
    int V;
    int E;
    std::vector<Airport> airports;
    std::vector<Edge*> adjList;
    std::vector<Edge*> undirAdjList;
    std::vector<UndirectedEdge> undirEdges;

    int addOrGetAirport(const std::string& code, const std::string& city);
    void addEdge(int src, int dest, double distance, double cost);

    void dijkstra(int src,
                  std::vector<double>& dist,
                  std::vector<double>& costArr,
                  std::vector<int>& parent) const;

    PathResult buildPath(int dest,
                         const std::vector<double>& dist,
                         const std::vector<double>& costArr,
                         const std::vector<int>& parent) const;

    static std::string extractState(const std::string& city);
    static void sortConnections(std::vector<ConnectionInfo>& arr);
    bool isUndirectedConnected() const;
};

#endif