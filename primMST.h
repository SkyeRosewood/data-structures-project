#ifndef PRIMMST_H
#define PRIMMST_H
#include "graph.h"
#include "minHeap.h"
//PRIMMST is a class to encapsulate Prim's minimum spanning Tree algorithm
class PrimMST {
public:
    // Runs Prim's algorithm on the undirected version of the graph
    // Returns the edges that make up the MST and the total cost in the MSTResult Struct
    static MSTResult run(const Graph& graph);
};

#endif
