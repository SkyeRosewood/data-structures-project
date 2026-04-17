#ifndef GRAPH_H
#define GRAPH_H
#include "vertex.h"
#include "edge.h"
#include <vector>

template <typename T>
class Graph{
    public:
        Graph() {}

        void insert_vertex(const Vertex<T>& ver);
        void add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int cost, int distance); //connect ver1 with ver2

        void print() const;

        void DFS(Vertex<T>& ver);
        void BFS(Vertex<T>& ver);
        int dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest);
        int kruskal_algorithim(const Vertex<T>& src, const Vertex<T>& dest);

    private:
        std::vector<Vertex<T>> vertices; //nodes
        std::vector<std::vector<Edge>> edges; //connections

        void clean_visited();

        void DFS_helper(Vertex<T>& ver);
        int get_vertex_index(const Vertex<T>& ver);
};

#endif