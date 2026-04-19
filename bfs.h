#ifndef BFS_H
#define BFS_H
#include "graph.h"
#include <vector>
#include <string>
using namespace std;

class BFS {
public:
    static vector<string> run(const Graph& graph, const string& startCode);
};

#endif