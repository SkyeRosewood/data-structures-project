#include "minHeap.h"

HeapNode::HeapNode() : index(-1), key(0.0), parent(-1) {}
HeapNode::HeapNode(int index, double key, int parent)
    : index(index), key(key), parent(parent) {}

MinHeap::MinHeap(int capacity) : capacity(capacity), currentSize(0) {
    data = new HeapNode[capacity];
    pos = new int[capacity];
    // Initialize inverse mapping to -1 (meaning the vertex is not currently in the heap)
    for (int i = 0; i < capacity; ++i) pos[i] = -1;
}

MinHeap::~MinHeap() { delete[] data; delete[] pos; }

bool MinHeap::isEmpty() const { return currentSize == 0; }
int MinHeap::size() const { return currentSize; }
bool MinHeap::contains(int v) const { return (v>=0&&v<capacity) && pos[v]!=-1; }

void MinHeap::insert(const HeapNode& node) {
    if (currentSize >= capacity) throw std::overflow_error("MinHeap full");
    
    // Place new node at the end of the array and update its inverse mapping
    int i = currentSize++;
    data[i] = node; 
    pos[node.index] = i;
    
    // Bubble the node up the tree until the min-heap property is restored
    heapifyUp(i);
}

HeapNode MinHeap::extractMin() {
    if (isEmpty()) throw std::underflow_error("MinHeap empty");
    
    // The root of the tree (index 0) always holds the minimum value
    HeapNode min = data[0];
    pos[min.index] = -1; // Mark as removed
    
    // Move the last leaf node to the root and sift it down to its proper place
    if (--currentSize > 0) { 
        data[0] = data[currentSize]; 
        pos[data[0].index] = 0; 
        heapifyDown(0); 
    }
    
    return min;
}

void MinHeap::decreaseKey(int v, double newKey, int newParent) {
    // Instantly locate the vertex in the heap array using the inverse mapping
    int i = pos[v];
    
    // Only update if the new cost is strictly cheaper than the current cost
    if (i==-1 || newKey >= data[i].key) return;
    
    data[i].key = newKey; 
    data[i].parent = newParent;
    
    // Because the key decreased, the node can only move UP the tree, never down
    heapifyUp(i);
}

void MinHeap::heapifyUp(int i) {
    // Compare child to parent, swapping if the child is smaller
    while (i > 0) {
        int p = (i-1)/2;
        if (data[p].key > data[i].key) { 
            swap(p,i); 
            i=p; 
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int i) {
    // Compare parent to both children, swapping with the smallest child
    while (true) {
        int s = i;          // Smallest node index
        int l = 2*i + 1;    // Left child index
        int r = 2*i + 2;    // Right child index
        
        if (l < currentSize && data[l].key < data[s].key) s = l;
        if (r < currentSize && data[r].key < data[s].key) s = r;
        
        // Stop if the parent is already smaller than both children
        if (s == i) break;
        
        swap(i, s); 
        i = s;
    }
}

void MinHeap::swap(int i, int j) {
    //update the inverse mapping array BEFORE swapping the actual nodes
    pos[data[i].index] = j; 
    pos[data[j].index] = i;
    
    HeapNode tmp = data[i]; 
    data[i] = data[j]; 
    data[j] = tmp;
}