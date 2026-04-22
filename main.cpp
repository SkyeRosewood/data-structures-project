#include "graph.h"
#include "csvReader.h"
#include "dfs.h"
#include "bfs.h"
#include "primMST.h"
#include "kruskalMST.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;


// print helpers
static void sep(const string& title) {
    cout << "\n" << string(60, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(60, '=') << "\n";
}

// Used for Tasks 2 and 3
static void printPath(const PathResult& pr, const string& origin, const string& dest) {
    if (!pr.found) {
        cout << "  Shortest route from " << origin << " to " << dest << ": None\n";
        return;
    }
    cout << "  Route: ";
    for (int i = 0; i < (int)pr.path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << pr.path[i];
    }
    cout << fixed << setprecision(2)
         << "  |  Distance: " << pr.distance
         << " mi  |  Cost: $" << pr.cost << "\n";
}

// Specifically handles the exact text formatting required for Task 4
static void printPathWithStops(const PathResult& pr, const string& origin, const string& dest, int stops) {
    if (!pr.found) {
        cout << "  Shortest route from " << origin << " to " << dest << " with " << stops << " stops: None\n";
        return;
    }
    cout << "  Shortest route from " << origin << " to " << dest << " with " << stops << " stops: ";
    for (int i = 0; i < (int)pr.path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << pr.path[i];
    }
    // Dropping decimals here to perfectly match the Task 4 sample output
    cout << fixed << setprecision(0)
         << ". The length is " << pr.distance
         << ". The cost is " << pr.cost << ".\n";
}

static void printTraversal(const vector<string>& order, const string& label) {
    cout << "  " << label << ": ";
    for (int i = 0; i < (int)order.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << order[i];
    }
    cout << "\n";
}

//Main

int main() {
    // Initialize the primary graph structure and populate it from the dataset
    Graph graph;
    if (!CSVReader::load("airports.txt", graph)) return 1;
    cout << "Loaded " << graph.getVertexCount() << " airports, "
         << graph.getEdgeCount() << " flights.\n";

    // ── Task 2: Shortest path between two airports ────────────────────────────
    // Standard pathfinding using Dijkstra's to find the absolute cheapest route
    sep("TASK 2 — Shortest Path Between Two Airports");
    printPath(graph.shortestPath("ATL", "MIA"), "ATL", "MIA");
    printPath(graph.shortestPath("MIA", "ORD"), "MIA", "ORD");
    printPath(graph.shortestPath("ORD", "BOS"), "ORD", "BOS");

    // ── Task 3: Shortest paths to all airports in a state ────────────────────
    // Batch pathfinding: tests the ability to filter destinations by state
    sep("TASK 3 — Shortest Paths from ATL to all FL airports");
    {
        auto results = graph.shortestPathsToState("ATL", "FL");
        if (results.empty()) { cout << "  No paths found.\n"; }
        else {
            for (const auto& pr : results)
                printPath(pr, "ATL", pr.destCode);
        }
    }
    sep("TASK 3 — Shortest Paths from ORD to all TX airports");
    {
        auto results = graph.shortestPathsToState("ORD", "TX");
        if (results.empty()) { cout << "  No paths found.\n"; }
        else {
            for (const auto& pr : results)
                printPath(pr, "ORD", pr.destCode);
        }
    }

    // ── Task 4: Shortest path with exact number of stops ─────────────────────
    //Tests paths that must strictly adhere to a step/depth count
    sep("TASK 4 — Shortest Path with Exact Stops");
    printPathWithStops(graph.shortestPathWithStops("ATL", "MIA",  1), "ATL", "MIA", 1);
    printPathWithStops(graph.shortestPathWithStops("MIA", "ORD",  1), "MIA", "ORD", 1);
    printPathWithStops(graph.shortestPathWithStops("ORD", "BOS",  2), "ORD", "BOS", 2);

    // ── Task 5: Connection counts ─────────────────────────────────────────────
    // Analyzes vertex degrees (in-degree and out-degree) to identify airports with most connections
    sep("TASK 5 — Top 10 Airports by Total Connections");
    auto conns = graph.getConnectionCounts();
    cout << left << setw(10) << "Airport"
                 << setw(10) << "Inbound"
                 << setw(10) << "Outbound"
                 << setw(10) << "Total" << "\n";
    cout << string(40, '-') << "\n";
    int limit = (int)conns.size() < 10 ? (int)conns.size() : 10;
    for (int i = 0; i < limit; ++i)
        cout << setw(10) << conns[i].code
             << setw(10) << conns[i].inbound
             << setw(10) << conns[i].outbound
             << setw(10) << conns[i].total << "\n";

    // ── Task 6 + 7 + 8 ───────────────────────────────────────────────────────
    // Flights are inherently directed, but MST algorithms require undirected edges
    sep("TASK 6 — Building Undirected Graph G_u");
    graph.buildUndirectedGraph();
    cout << "  G_u built. Connected: " << (graph.isConnected() ? "Yes" : "No") << "\n";
    if (!graph.isConnected())
        cout << "  (Graph is disconnected — Prim's will fail, Kruskal's will produce a spanning forest)\n";

    // Compare two different MST algorithms to ensure they compute identical total costs
    sep("TASK 7 — Prim's MST (first 10 edges shown)");
    {
        MSTResult mst = PrimMST::run(graph);
        // Correctly fails out and provides the required message if graph is disconnected
        if (!mst.isConnected) {
            cout << "  MST cannot be formed.\n";
        } else {
            int show = (int)mst.edges.size() < 10 ? (int)mst.edges.size() : 10;
            for (int i = 0; i < show; ++i)
                cout << "    " << mst.edges[i].from << " - " << mst.edges[i].to
                     << "  $" << fixed << setprecision(2) << mst.edges[i].cost << "\n";
            cout << "  ... (" << mst.edges.size() << " edges total)\n";
            cout << "  Total Cost: $" << fixed << setprecision(2) << mst.totalCost << "\n";
        }
    }

    sep("TASK 8 — Kruskal's MST (first 10 edges shown)");
    {
        MSTResult mst = KruskalMST::run(graph);
        if (!mst.isConnected) {
            cout << "  Minimal Spanning Forest (graph is disconnected):\n"; // Changed this line
        } else {
            cout << "  Minimal Spanning Tree:\n";
        }
        
        int show = (int)mst.edges.size() < 10 ? (int)mst.edges.size() : 10;
        for (int i = 0; i < show; ++i)
            cout << "    " << mst.edges[i].from << " - " << mst.edges[i].to
                 << "  $" << fixed << setprecision(2) << mst.edges[i].cost << "\n";
        cout << "  ... (" << mst.edges.size() << " edges total)\n";
        cout << "  Total Cost: $" << fixed << setprecision(2) << mst.totalCost << "\n";
    }

    // ── DFS & BFS ────────────────────────────────────────────────────────────
    // Standard unweighted network traversals to verify graph connectivity and exploration order
    sep("DFS Traversal");
    printTraversal(DFS::run(graph, "ATL"), "DFS from ATL");
    printTraversal(DFS::run(graph, "MIA"), "DFS from MIA");
    printTraversal(DFS::run(graph, "ORD"), "DFS from ORD");

    sep("BFS Traversal");
    printTraversal(BFS::run(graph, "ATL"), "BFS from ATL");
    printTraversal(BFS::run(graph, "MIA"), "BFS from MIA");
    printTraversal(BFS::run(graph, "ORD"), "BFS from ORD");

    return 0;
}
