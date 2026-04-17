#ifndef EDGE_H
#define EDGE_H

class Edge {
    public:
        
        Edge(int src = 0, int dest = 0, int cost = 0, int distance = 0) : 
            src(src), dest(dest), cost(cost), distance(distance) {}
        
        int src;
        int dest;
        int cost;
        int distance; 
};

// Custom comparators

struct CompareEdgeByCost {
    bool operator()(const Edge& a, const Edge& b) const {
        return a.cost < b.cost;
    }
};

struct CompareEdgeByDistance {
    bool operator()(const Edge& a, const Edge& b) const {
        return a.distance < b.distance;
    }
};

#endif