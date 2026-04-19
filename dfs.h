#ifndef DFS_H
#define DFS_H
#include "graph.h"
#include <vector>
#include <string>
using namespace std;

class DFS {
public:
    static vector<string> run(const Graph& graph, const string& startCode);
};

#endif