#include "dfs.h"
#include "stack.h"
#include <vector>
#include <string>
using namespace std;

// Depth First Search
// Returns a vector Node in order they were visited
vector<string> DFS::run(const Graph& graph, const string& startCode) {
    vector<string> order;
    int V = graph.getVertexCount();
    int start = graph.indexOf(startCode);

    // If the starting node is not founds, throws an error
    if (start == -1) return order;

    // Tracks which nodes have already been visited
    // Uses stacks
    vector<bool> visited(V, false);
    Stack stack;

    stack.push(start);
    while (!stack.isEmpty()) {
        int u = stack.pop();

        // Skips the node if it has already been visited
        // Marks node after visited
        if (visited[u]) continue;
        visited[u] = true;
        order.push_back(graph.codeOf(u));

        // pushes all unvisited nodes back onto the stack
        for (Edge* e = graph.getAdjHead(u); e; e = e->next)
            if (!visited[e->dest]) stack.push(e->dest);
    }
    return order;
}