#ifndef PRIMMST_H
#define PRIMMST_H
#include "graph.h"
#include "minHeap.h"

class PrimMST {
public:
    static MSTResult run(const Graph& graph);
};

#endif