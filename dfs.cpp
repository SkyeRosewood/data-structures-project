#include "dfs.h"
#include "stack.h"
#include <vector>
#include <string>

using namespace std;

vector<string> DFS::run(const Graph& graph, const string& startCode) {
    vector<string> order;
    int V     = graph.getVertexCount();
    int start = graph.indexOf(startCode);
    if (start == -1) return order;

    vector<bool> visited(V, false);
    Stack stack;

    stack.push(start);
    while (!stack.isEmpty()) {
        int u = stack.pop();
        if (visited[u]) continue;
        visited[u] = true;
        order.push_back(graph.codeOf(u));
        for (Edge* e = graph.getAdjHead(u); e; e = e->next)
            if (!visited[e->dest]) stack.push(e->dest);
    }
    return order;
}