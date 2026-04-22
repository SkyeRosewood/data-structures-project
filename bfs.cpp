#include "bfs.h"
#include "queue.h"
#include <vector>
#include <string>
using namespace std;

vector<string> BFS::run(const Graph& graph, const string& startCode) {
    vector<string> order;
    int V     = graph.getVertexCount();
    int start = graph.indexOf(startCode);
    if (start == -1) return order;

    vector<bool> visited(V, false);
    Queue queue;

    visited[start] = true;
    queue.enqueue(start);

    while (!queue.isEmpty()) {
        int u = queue.dequeue();
        order.push_back(graph.codeOf(u));
        for (Edge* e = graph.getAdjHead(u); e; e = e->next)
            if (!visited[e->dest]) { visited[e->dest]=true; queue.enqueue(e->dest); }
    }
    return order;
}