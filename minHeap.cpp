#include "minHeap.h"

HeapNode::HeapNode() : index(-1), key(0.0), parent(-1) {}
HeapNode::HeapNode(int index, double key, int parent)
    : index(index), key(key), parent(parent) {}

MinHeap::MinHeap(int capacity) : capacity(capacity), currentSize(0) {
    data = new HeapNode[capacity];
    pos = new int[capacity];
    for (int i = 0; i < capacity; ++i) pos[i] = -1;
}

MinHeap::~MinHeap() { delete[] data; delete[] pos; }

bool MinHeap::isEmpty() const { return currentSize == 0; }
int MinHeap::size() const { return currentSize; }
bool MinHeap::contains(int v) const { return (v>=0&&v<capacity) && pos[v]!=-1; }

void MinHeap::insert(const HeapNode& node) {
    if (currentSize >= capacity) throw std::overflow_error("MinHeap full");
    int i = currentSize++;
    data[i] = node; pos[node.index] = i;
    heapifyUp(i);
}

HeapNode MinHeap::extractMin() {
    if (isEmpty()) throw std::underflow_error("MinHeap empty");
    HeapNode min = data[0];
    pos[min.index] = -1;
    if (--currentSize > 0) { data[0]=data[currentSize]; pos[data[0].index]=0; heapifyDown(0); }
    return min;
}

void MinHeap::decreaseKey(int v, double newKey, int newParent) {
    int i = pos[v];
    if (i==-1 || newKey >= data[i].key) return;
    data[i].key=newKey; data[i].parent=newParent;
    heapifyUp(i);
}

void MinHeap::heapifyUp(int i) {
    while (i > 0) {
        int p = (i-1)/2;
        if (data[p].key > data[i].key) { swap(p,i); i=p; } else break;
    }
}

void MinHeap::heapifyDown(int i) {
    while (true) {
        int s=i, l=2*i+1, r=2*i+2;
        if (l<currentSize && data[l].key<data[s].key) s=l;
        if (r<currentSize && data[r].key<data[s].key) s=r;
        if (s==i) break;
        swap(i,s); i=s;
    }
}

void MinHeap::swap(int i, int j) {
    pos[data[i].index]=j; pos[data[j].index]=i;
    HeapNode tmp=data[i]; data[i]=data[j]; data[j]=tmp;
}