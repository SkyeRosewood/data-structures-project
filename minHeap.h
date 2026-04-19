#ifndef MINHEAP_H
#define MINHEAP_H
#include <stdexcept>

struct HeapNode {
    int index;
    double key;
    int parent;

    HeapNode();
    HeapNode(int index, double key, int parent = -1);
};

class MinHeap {
public:
    explicit MinHeap(int capacity);
    ~MinHeap();

    void insert(const HeapNode& node);
    HeapNode extractMin();
    void decreaseKey(int vertexIndex, double newKey, int newParent);
    bool contains(int vertexIndex) const;
    bool isEmpty() const;
    int size() const;

private:
    HeapNode* data;
    int* pos;
    int capacity;
    int currentSize;

    void heapifyUp(int i);
    void heapifyDown(int i);
    void swap(int i, int j);
};

#endif