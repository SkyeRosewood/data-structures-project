#ifndef MINHEAP_H
#define MINHEAP_H
#include <stdexcept>

// Represents a vertex and its current shortest distance/cost in the graph
struct HeapNode {
    int index;      // graph vertex ID
    double key;     // The priority value (e.g., edge cost or distance)
    int parent;     // Tracks the path for reconstructing the MST/shortest path later

    HeapNode();
    HeapNode(int index, double key, int parent = -1);
};

// A  Priority Queue for Prim's or Dijkstra's algorithm
class MinHeap {
public:
    explicit MinHeap(int capacity);
    ~MinHeap();

    // Core priority operations
    void insert(const HeapNode& node);
    HeapNode extractMin(); // Retrieves and removes the lowest-cost node in O(log V) time
    
    // Updates a vertex with a cheaper cost and perculates up the tree
    void decreaseKey(int vertexIndex, double newKey, int newParent);
    
    // Read-only utility methods
    bool contains(int vertexIndex) const;
    bool isEmpty() const;
    int size() const;

private:
    HeapNode* data;     
    
    int* pos;           
    
    int capacity;
    int currentSize;

    // percolates up and down in order to retain minHeap functionality
    void heapifyUp(int i);
    void heapifyDown(int i);
    void swap(int i, int j);
};

#endif