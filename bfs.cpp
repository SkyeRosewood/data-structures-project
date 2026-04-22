#include "bfs.h"
#include "queue.h"
#include <vector>
#include <string>
using namespace std;

// Breadth First Search
// Returns a vector Node in order they were visited
vector<string> BFS::run(const Graph& graph, const string& startCode) {
    vector<string> order;
    int V = graph.getVertexCount();
    int start = graph.indexOf(startCode);

    // If the starting node is not found, throws an error
    if (start == -1) return order;

    // Tracks which nodes have already been visited
    // Uses queues
    vector<bool> visited(V, false);
    Queue queue;

    visited[start] = true;
    queue.enqueue(start);

    // Records viseted nodes in traversal order
    while (!queue.isEmpty()) {
        int u = queue.dequeue();
        order.push_back(graph.codeOf(u));

        // Explores all adjasent nodes
        for (Edge* e = graph.getAdjHead(u); e; e = e->next)
            if (!visited[e->dest]) { 

                // Marks Node as visited before enqueueing it
                visited[e->dest] = true; 
                queue.enqueue(e->dest); 
            }
    }
    return order;
}