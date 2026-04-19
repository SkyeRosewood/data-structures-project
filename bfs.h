#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include <vector>
#include <string>

class BFS {
public:
    static std::vector<std::string> run(const Graph& graph,
                                        const std::string& startCode);
};

#endif