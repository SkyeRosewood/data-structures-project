#ifndef DFS_H
#define DFS_H
#include "graph.h"
#include <vector>
#include <string>

class DFS {
public:
    static std::vector<std::string> run(const Graph& graph,
                                        const std::string& startCode);
};

#endif